#ifndef ORDER_DETAILS_IMPL_HPP
#define ORDER_DETAILS_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

namespace order_details {

/*--------------------------------------------OrderDetails-----------------------------------------------------*/
template <typename Order>
OrderDetails<Order>::OrderDetails(MarketMember* market_member_ptr,
                                  std::size_t num_units,
                                  OrderType::PaymentCurrencyType unit_price,
                                  TradingSide side)
    : market_member_ptr_(market_member_ptr),
      num_units_(num_units),
      unit_price_(unit_price),
      side_(side) {
}

}  // namespace order_details

}  // namespace models

TRADING_NAMESPACE_END

#endif