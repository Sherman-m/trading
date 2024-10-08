#ifndef ORDER_DETAILS_HPP
#define ORDER_DETAILS_HPP

#include <cstdint>
#include <trading/models/market_member_fwd.hpp>
#include <trading/models/orders_matching_fwd.hpp>
#include <trading/models/trading_side.hpp>

TRADING_NAMESPACE_BEGIN

namespace models {

namespace order_details {

/*--------------------------------------------OrderBase--------------------------------------------------------*/
class OrderBase {
 public:
  using ID = std::uint32_t;
};

/*--------------------------------------------OrderDetails-----------------------------------------------------*/
template <typename Order>
class OrderDetails {
  friend class OrderBase;
  friend class OrdersMatching<Order>;

 public:
  using OrderType = Order;

  explicit OrderDetails(MarketMember* market_member_ptr, std::size_t num_units,
                        OrderType::PaymentCurrencyType unit_price,
                        TradingSide side);

  const MarketMember* MarketMemberPtr() const noexcept {
    return market_member_ptr_;
  }

  MarketMember* MarketMemberPtr() noexcept {
    return market_member_ptr_;
  }

  std::size_t NumUnits() const noexcept {
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
  std::size_t num_units_;
  OrderType::PaymentCurrencyType unit_price_;
  TradingSide side_;
};

}  // namespace order_details

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/order_details.hpp>

#endif