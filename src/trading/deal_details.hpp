#ifndef DEAL_DETAILS_HPP
#define DEAL_DETAILS_HPP

#include <cinttypes>

#include "market_member_fwd.hpp"
#include "trading_side.hpp"

namespace trading {

namespace deal_details {

/*---------------------------------------------DealBase--------------------------------------------------------*/
class DealBase {
 public:
  using ID = uint32_t;
};

/*---------------------------------------------DealPart--------------------------------------------------------*/
template <typename Deal>
class DealPart {
 public:
  using DealType = Deal;
  using OrderID = Deal::OrderType::ID;

 public:
  explicit DealPart(MarketMember* market_member_ptr, OrderID order_id,
                    size_t num_units, DealType::PaymentCurrencyType paid,
                    TradingSide side);

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

  DealType::PaymentCurrencyType Paid() const noexcept {
    return paid_;
  }

  TradingSide Side() const noexcept {
    return side_;
  }

  friend auto operator<=>(const DealPart&, const DealPart&) = default;

 private:
  MarketMember* market_member_ptr_;
  OrderID order_id_;
  size_t num_units_;
  DealType::PaymentCurrencyType paid_;
  TradingSide side_;
};

}  // namespace deal_details

}  // namespace trading

#include "impl/deal_details_impl.hpp"

#endif