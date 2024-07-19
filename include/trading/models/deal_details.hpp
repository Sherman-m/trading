#ifndef DEAL_DETAILS_HPP
#define DEAL_DETAILS_HPP

#include <cstdint>
#include <trading/models/market_member_fwd.hpp>
#include <trading/models/trading_side.hpp>

TRADING_NAMESPACE_BEGIN

namespace models {

namespace deal_details {

/*---------------------------------------------DealBase--------------------------------------------------------*/
class DealBase {
 public:
  using ID = std::uint32_t;
};

/*---------------------------------------------DealPart--------------------------------------------------------*/
template <typename Deal>
class DealPart {
 public:
  using DealType = Deal;
  using OrderID = Deal::OrderType::ID;
  using MarketMemberID = DealType::OrderType::MarketMemberType::ID;

  explicit DealPart(MarketMemberID market_member_ptr, OrderID order_id,
                    std::size_t num_units, DealType::PaymentCurrencyType paid,
                    TradingSide side);

  const MarketMemberID MarketMemberId() const noexcept {
    return market_member_id_;
  }

  MarketMemberID MarketMemberId() noexcept {
    return market_member_id_;
  }

  OrderID OrderId() const noexcept {
    return order_id_;
  }

  std::size_t NumUnits() const noexcept {
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
  MarketMemberID market_member_id_;
  OrderID order_id_;
  std::size_t num_units_;
  DealType::PaymentCurrencyType paid_;
  TradingSide side_;
};

}  // namespace deal_details

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/deal_details.hpp>

#endif