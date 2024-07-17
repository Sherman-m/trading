#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <optional>

#include "deal_fwd.hpp"
#include "order_fwd.hpp"

namespace trading {

namespace storages {

template <typename Order, typename Deal>
class IStorage {
 public:
  using OrderType = Order;
  using DealType = Deal;

  virtual ~IStorage() = default;

  virtual void AddOrder(OrderType order) = 0;
  virtual std::optional<OrderType> GetOrder(
      typename OrderType::ID id) const = 0;

  virtual void AddDeal(DealType deal) = 0;
  virtual std::optional<DealType> GetDeal(typename DealType::ID id) const = 0;

  virtual void FindMatchingOrders() = 0;
};

}  // namespace storages

}  // namespace trading

#endif