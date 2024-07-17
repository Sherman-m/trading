#ifndef ORDER_HPP
#define ORDER_HPP

#include <trading/models/order_details.hpp>
#include <trading/models/orders_matching.hpp>
#include <trading/models/segment_fwd.hpp>

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename TargetCurrency, typename PaymentCurrency>
class Order : public order_details::OrderBase {
  friend class OrdersMatching<Order>;

 public:
  using BaseType = order_details::OrderBase;
  using ID = BaseType::ID;

  using TargetCurrencyType = TargetCurrency;
  using PaymentCurrencyType = PaymentCurrency;

  using DetailsType = order_details::OrderDetails<Order>;
  using MatchingType = OrdersMatching<Order>;
  using SegmentType = Segment<TargetCurrency, PaymentCurrency>;

  explicit Order(ID id, DetailsType details);

  ID Id() const noexcept {
    return id_;
  }

  const DetailsType& Details() const noexcept {
    return details_;
  }

  DetailsType& Details() noexcept {
    return details_;
  }

  bool IsCompleted() const noexcept {
    return details_.NumUnits() == 0;
  }

  friend bool operator==(const Order& first, const Order& second) {
    return first.id_ == second.id_ && first.details_ == second.details_;
  }

  friend auto operator<=>(const Order&, const Order&) = default;

 private:
  ID id_;
  DetailsType details_;
};

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/order.hpp>

#endif