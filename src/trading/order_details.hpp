#ifndef ORDER_DETAILS_HPP
#define ORDER_DETAILS_HPP

#include <cinttypes>

#include "market_member_fwd.hpp"
#include "orders_matching_fwd.hpp"
#include "trading_side.hpp"

namespace trading {

namespace order_details {

/*--------------------------------------------OrderBase--------------------------------------------------------*/
class OrderBase {
 public:
  using ID = uint32_t;
};

/*--------------------------------------------OrderDetails-----------------------------------------------------*/
template <typename Order>
class OrderDetails {
  friend class OrderBase;
  friend class OrdersMatching<Order>;

 public:
  using OrderType = Order;

 public:
  explicit OrderDetails(MarketMember* market_member_ptr, size_t num_units,
                        OrderType::PaymentCurrencyType unit_price,
                        TradingSide side);

  const MarketMember* MarketMemberPtr() const noexcept {
    return market_member_ptr_;
  }

  MarketMember* MarketMemberPtr() noexcept {
    return market_member_ptr_;
  }

  size_t NumUnits() const noexcept {
    return num_units_;
  }

  OrderType::PaymentCurrencyType UnitPrice() const noexcept {
    return unit_price_;
  }

  TradingSide Side() const noexcept {
    return side_;
  }

  friend auto operator<=>(const OrderDetails&, const OrderDetails&) = default;

 private:
  MarketMember* market_member_ptr_;
  size_t num_units_;
  OrderType::PaymentCurrencyType unit_price_;
  TradingSide side_;
};

}  // namespace order_details

}  // namespace trading

#include "impl/order_details_impl.hpp"

#endif