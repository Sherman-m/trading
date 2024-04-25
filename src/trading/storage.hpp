#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "order_fwd.hpp"
#include "deal_fwd.hpp"
#include <optional>

namespace trading {

namespace storages {

template <typename TargetCurrency, typename PaymentCurrency>
class IStorage {
 public:
  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using DealType = Deal<TargetCurrency, PaymentCurrency>;

 public:
  virtual ~IStorage() = default;

  virtual void AddOrder(OrderType order) = 0;
  virtual std::optional<OrderType> GetOrder(OrderType::ID id) const = 0;

  virtual void AddDeal(DealType deal) = 0;
  virtual std::optional<DealType> GetDeal(DealType::ID id) const = 0;

  virtual void FindMatchingOrders() = 0;
};

}  // namespace storages

}  // namespace trading

#endif