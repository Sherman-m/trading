#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <optional>
#include <trading/models/deal_fwd.hpp>
#include <trading/models/order_fwd.hpp>

TRADING_NAMESPACE_BEGIN

namespace storages {

template <typename Config, typename IDType>
class IStorage {
  struct OrderConfig {
    using ID = IDType;
    using TargetCurrencyType = typename Config::TargetCurrencyType;
    using PaymentCurrencyType = typename Config::PaymentCurrencyType;
    using SegmentType = typename Config::SegmentType;
    using MarketMemberType = typename Config::MarketMemberType;
  };

 public:
  using OrderType = models::Order<OrderConfig>;
  using DealType = models::Deal<OrderType>;

  virtual ~IStorage() = default;

  virtual OrderType::ID AddOrder(OrderType::DetailsType order) = 0;
  virtual std::optional<OrderType> GetOrder(OrderType::ID id) const = 0;

  virtual DealType::ID AddDeal(
      DealType::OrderType::MatchingType::Result matching_result) = 0;
  virtual std::optional<DealType> GetDeal(DealType::ID id) const = 0;

  virtual void FindMatchingOrders() = 0;
};

}  // namespace storages

TRADING_NAMESPACE_END

#endif