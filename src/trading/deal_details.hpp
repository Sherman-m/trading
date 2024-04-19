#ifndef DEAL_DETAILS_HPP
#define DEAL_DETAILS_HPP

#include <cinttypes>
#include "order_details.hpp"
#include "market_member_fwd.hpp"

namespace trading {

namespace deal_details {

struct DealBase {
  using Id = uint32_t;
};

enum class DealSide : int32_t { kBuy = 1, kSale = -1 };

template <typename TargetCurrency, typename PaymentCurrency>
struct DealPart {
  using OrderId = order_details::OrderBase::Id;

  explicit DealPart(MarketMember* market_member_ptr, OrderId order_id,
                    size_t num_units, PaymentCurrency paid, DealSide side)
      : market_member_ptr_(market_member_ptr),
        order_id(order_id),
        num_units(num_units),
        paid(paid),
        side(side) {
  }

  MarketMember* market_member_ptr_;
  OrderId order_id;
  size_t num_units;
  PaymentCurrency paid;
  DealSide side;
};

}  // namespace deal_details

}  // namespace trading

#endif