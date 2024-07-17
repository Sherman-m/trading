#ifndef ORDER_HPP
#define ORDER_HPP

#include "order_details.hpp"
#include "orders_matching.hpp"
#include "segment_fwd.hpp"

namespace trading {

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

}  // namespace trading

#include "impl/order_impl.hpp"

#endif