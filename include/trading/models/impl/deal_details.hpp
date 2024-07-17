#ifndef DEAL_DETAILS_IMPL_HPP
#define DEAL_DETAILS_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

namespace deal_details {

/*---------------------------------------------DealPart--------------------------------------------------------*/
template <typename Deal>
DealPart<Deal>::DealPart(MarketMember* market_member_ptr, OrderID order_id,
                         std::size_t num_units,
                         DealType::PaymentCurrencyType paid, TradingSide side)
    : market_member_ptr_(market_member_ptr),
      order_id_(order_id),
      num_units_(num_units),
      paid_(paid),
      side_(side) {
}

}  // namespace deal_details

}  // namespace models

TRADING_NAMESPACE_END

#endif