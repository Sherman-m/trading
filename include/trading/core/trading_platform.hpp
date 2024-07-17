#ifndef TRADING_PLATFORM_HPP
#define TRADING_PLATFORM_HPP

#include <trading/models/currency.hpp>
#include <trading/models/deal.hpp>
#include <trading/models/market_member_fwd.hpp>
#include <trading/models/order.hpp>
#include <trading/models/segment.hpp>
#include <trading/models/trading_side.hpp>
#include <utility>

TRADING_NAMESPACE_BEGIN

namespace core {

/*-------------------------------------TradingPlatformScopeFWD-------------------------------------------------*/
class TradingPlatformScope;

/*---------------------------------------TradingPlatformImpl---------------------------------------------------*/
namespace trading_platform_details {

class TradingPlatformImpl {
  friend class trading::core::TradingPlatformScope;

 public:
  using OrderID = models::order_details::OrderBase::ID;
  using DealID = models::deal_details::DealBase::ID;

  template <typename Order>
  OrderID PublishOrder(Order::DetailsType order_details);

  template <typename Deal>
  void CloseDeal(Deal::OrderType::MatchingType::Result matching_res);

 private:
  template <typename TargetCurrency, typename PaymentCurrency>
  using SegmentTemplate = models::Segment<TargetCurrency, PaymentCurrency>;
  using TupleSegmentsType =
      models::currencies::TupleObj2ArgsFromCurrencyTypesT<SegmentTemplate>;

  template <template <typename, typename> typename Storage>
  explicit TradingPlatformImpl(const models::SegmentConfig<Storage>& config);

  ~TradingPlatformImpl() = default;

  TupleSegmentsType segments_;
  OrderID next_order_id_{0};
  DealID next_deal_id_{0};
};

}  // namespace trading_platform_details

/*---------------------------------------TradingPlatformScope--------------------------------------------------*/
class TradingPlatformScope {
 public:
  using TradingPlatformImpl = trading_platform_details::TradingPlatformImpl;

  TradingPlatformScope() = delete;

  TradingPlatformScope(const TradingPlatformScope&) = delete;
  TradingPlatformScope& operator=(const TradingPlatformScope&) = delete;

  TradingPlatformScope(TradingPlatformScope&&) = delete;
  TradingPlatformScope& operator=(TradingPlatformScope&&) = delete;

  ~TradingPlatformScope() = default;

  template <template <typename, typename> typename Storage>
  static void Run(const models::SegmentConfig<Storage>& config);

  static TradingPlatformImpl* Get();

  static void Stop();

 private:
  static TradingPlatformImpl* trading_platform_ptr_;
};

}  // namespace core

TRADING_NAMESPACE_END

#include <trading/core/impl/trading_platform.hpp>

#endif