#ifndef ORDER_DETAILS_HPP
#define ORDER_DETAILS_HPP

#include <cinttypes>
#include <stdexcept>
#include "market_member_fwd.hpp"

namespace trading {

template <typename TargetCurrency, typename PaymentCurrency>
class OrdersMatching;

namespace order_details {

struct OrderBase {
  using ID = uint32_t;
};

enum class OrderSide : int32_t { kBuy = 1, kSale = -1 };

template <typename TargetCurrency, typename PaymentCurrency>
class OrderDetails {
  friend struct OrderBase;
  friend class OrdersMatching<TargetCurrency, PaymentCurrency>;

 public:
  explicit OrderDetails(MarketMember* market_member_ptr, size_t num_units,
                        PaymentCurrency unit_price, OrderSide side)
      : market_member_ptr_(market_member_ptr),
        num_units_(num_units),
        unit_price_(unit_price),
        side_(side) {
  }

  const MarketMember* MarketMemberPtr() const noexcept {
    return market_member_ptr_;
  }

  MarketMember* MarketMemberPtr() noexcept {
    return market_member_ptr_;
  }

  size_t NumUnits() const noexcept {
    return num_units_;
  }

  PaymentCurrency UnitPrice() const noexcept {
    return unit_price_;
  }

  OrderSide Side() const noexcept {
    return side_;
  }

  friend auto operator<=>(const OrderDetails&, const OrderDetails&) = default;

 private:
  MarketMember* market_member_ptr_;
  size_t num_units_;
  PaymentCurrency unit_price_;
  OrderSide side_;
};

}  // namespace order_details

}  // namespace trading

#endif