#ifndef DEAL_DETAILS_HPP
#define DEAL_DETAILS_HPP

#include <cinttypes>
#include "order_details.hpp"
#include "market_member_fwd.hpp"

namespace trading {

namespace deal_details {

struct DealBase {
  using ID = uint32_t;
};

enum class DealSide : int32_t { kBuy = 1, kSale = -1 };

template <typename TargetCurrency, typename PaymentCurrency>
class DealPart {
 public:
  using OrderID = order_details::OrderBase::ID;

 public:
  explicit DealPart(MarketMember* market_member_ptr, OrderID order_id,
                    size_t num_units, PaymentCurrency paid, DealSide side)
      : market_member_ptr_(market_member_ptr),
        order_id_(order_id),
        num_units_(num_units),
        paid_(paid),
        side_(side) {
  }

  const MarketMember* MarketMemberPtr() const noexcept {
    return market_member_ptr_;
  }

  MarketMember* MarketMemberPtr() noexcept {
    return market_member_ptr_;
  }

  OrderID OrderId() const noexcept {
    return order_id_;
  }

  size_t NumUnits() const noexcept {
    return num_units_;
  }

  PaymentCurrency Paid() const noexcept {
    return paid_;
  }

  DealSide Side() const noexcept {
    return side_;
  }

  friend auto operator<=>(const DealPart&, const DealPart&) = default;

 private:
  MarketMember* market_member_ptr_;
  OrderID order_id_;
  size_t num_units_;
  PaymentCurrency paid_;
  DealSide side_;
};

}  // namespace deal_details

}  // namespace trading

#endif