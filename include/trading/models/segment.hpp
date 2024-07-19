#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <boost/asio/io_context.hpp>
#include <chrono>
#include <memory>
#include <optional>
#include <trading/models/deal_fwd.hpp>
#include <trading/models/order_fwd.hpp>
#include <trading/models/segment_analyzer.hpp>
#include <trading/storages/storage.hpp>

TRADING_NAMESPACE_BEGIN

namespace models {

/*----------------------------------------------Segment--------------------------------------------------------*/
template <typename Config, typename TargetCurrency, typename PaymentCurrency>
class Segment {
  struct StorageConfig {
    using MarketMemberType = typename Config::MarketMemberType;

    using TargetCurrencyType = TargetCurrency;
    using PaymentCurrencyType = PaymentCurrency;
    using TradingPlatformScopeType = typename Config::TradingPlatformScopeType;
    using SegmentType = Segment<Config, TargetCurrency, PaymentCurrency>;
  };

 public:
  using OrderType = typename Config::StorageType<StorageConfig>::OrderType;
  using DealType = typename Config::StorageType<StorageConfig>::DealType;

  Segment(Config& config);

  Segment(const Segment&) = delete;
  Segment& operator=(const Segment&) = delete;

  Segment(Segment&& other);
  Segment& operator=(Segment&& other);

  ~Segment();

  OrderType::ID AddOrder(OrderType::DetailsType order);

  std::optional<OrderType> GetOrder(OrderType::ID id) const;

  DealType::ID AddDeal(DealType::PartsType deal);

  std::optional<DealType> GetDeal(DealType::ID id) const;

  void Analyze();

 private:
  using SegmentAnalyzerType = segment_details::SegmentAnalyzer<
      Segment<Config, TargetCurrency, PaymentCurrency>>;

  using StorageType = typename Config::StorageType<StorageConfig>;

  std::unique_ptr<StorageType> storage_ptr_;
  std::shared_ptr<SegmentAnalyzerType> segment_analyzer_ptr_;
};

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/segment.hpp>

#endif