#ifndef TRADING_PLATFORM_HPP
#define TRADING_PLATFORM_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <trading/models/currency.hpp>
#include <trading/models/deal.hpp>
#include <trading/models/market_member_fwd.hpp>
#include <trading/models/order.hpp>
#include <trading/models/order_details.hpp>
#include <trading/models/segment.hpp>
#include <trading/models/trading_side.hpp>
#include <utility>

TRADING_NAMESPACE_BEGIN

namespace core {

/*-------------------------------------TradingPlatformScopeFWD-------------------------------------------------*/
template <typename Config>
class TradingPlatformScope;

/*---------------------------------------TradingPlatformImpl---------------------------------------------------*/
namespace trading_platform_details {

template <typename Config>
class TradingPlatformImpl {
  friend class trading::core::TradingPlatformScope<Config>;

  struct Con;
  struct SegmentConfig {
    template <typename StorageConfig>
    using StorageType = typename Config::StorageType<StorageConfig>;

    using MarketMemberType =
        typename Config::MarketMemberStorageType<Con>::MarketMemberType;

    using TradingPlatformScopeType = TradingPlatformScope<Config>;

    explicit SegmentConfig(Config& config)
        : config_(config) {
    }

    Config& config_;
  };
  template <typename TargetCurrency, typename PaymentCurrency>
  using SegmentTemplate =
      models::Segment<SegmentConfig, TargetCurrency, PaymentCurrency>;

  struct Con {
    using TradingPlatformScopeType = TradingPlatformScope<Config>;
    template <typename TargetCurrency, typename PaymentCurrency>
    using OrderType =
        typename SegmentTemplate<TargetCurrency, PaymentCurrency>::OrderType;
  };

 public:
  template <typename Order>
  Order::ID PublishOrder(Order::DetailsType order_details);

  template <typename Order>
  std::optional<Order> GetOrder(const Order::ID& id) const noexcept {
    return std::get<typename Order::SegmentType>(segments_).GetOrder(id);
  }

  template <typename Deal>
  void CloseDeal(Deal::OrderType::MatchingType::Result matching_result);

  template <typename Deal>
  std::optional<Deal> GetDeal(const Deal::ID& id) const noexcept {
    return std::get<typename Deal::SegmentType>(segments_).GetDeal(id);
  }

  typename Config::MarketMemberStorageType<Con>::MarketMemberType::ID
  AddMarketMember() {
    return mm_storage_.AddMarketMember();
  }

  typename Config::MarketMemberStorageType<Con>::MarketMemberType*
  GetMarketMember(
      typename Config::MarketMemberStorageType<Con>::MarketMemberType::ID id) {
    return mm_storage_.GetMarketMember(id);
  }

 private:
  using TupleSegmentsType =
      models::currencies::TupleObj2ArgsFromCurrencyTypesT<SegmentTemplate>;

  explicit TradingPlatformImpl(Config config);

  ~TradingPlatformImpl() = default;

  Config config_;
  TupleSegmentsType segments_;
  typename Config::MarketMemberStorageType<Con> mm_storage_;
};

}  // namespace trading_platform_details

/*---------------------------------------TradingPlatformScope--------------------------------------------------*/
template <typename Config>
class TradingPlatformScope {
 private:
  using TradingPlatformImpl =
      trading_platform_details::TradingPlatformImpl<Config>;

  using TradingPlatformPtr = TradingPlatformImpl*;

 public:
  TradingPlatformScope() = delete;

  TradingPlatformScope(const TradingPlatformScope&) = delete;
  TradingPlatformScope& operator=(const TradingPlatformScope&) = delete;

  TradingPlatformScope(TradingPlatformScope&&) = delete;
  TradingPlatformScope& operator=(TradingPlatformScope&&) = delete;

  ~TradingPlatformScope() = default;

  static void Run(Config config);

  static TradingPlatformPtr Get();

  static void Stop();

 private:
  static TradingPlatformPtr trading_platform_;
};

}  // namespace core

TRADING_NAMESPACE_END

#include <trading/core/impl/trading_platform.hpp>

#endif