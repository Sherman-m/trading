#ifndef ORDER_DETAILS_HPP
#define ORDER_DETAILS_HPP

#include <cinttypes>
#include "market_member_fwd.hpp"

namespace trading {

namespace order_details {

struct OrderBase {
  using Id = uint32_t;
};

enum class OrderSide : int32_t { kBuy = -1, kSale = 1 };

template <typename TargetCurrency, typename PaymentCurrency>
struct OrderDetails {
  explicit OrderDetails(MarketMember* market_member_ptr, size_t num_units,
                        PaymentCurrency unit_price, OrderSide side)
      : market_member_ptr(market_member_ptr),
        num_units(num_units),
        unit_price(unit_price),
        side(side) {
  }

  MarketMember* market_member_ptr;
  size_t num_units;
  PaymentCurrency unit_price;
  OrderSide side;
};
}  // namespace order_details

}  // namespace trading

#endif