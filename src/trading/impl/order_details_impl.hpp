#ifndef ORDER_DETAILS_IMPL_HPP
#define ORDER_DETAILS_IMPL_HPP

namespace trading {

namespace order_details {

/*--------------------------------------------OrderDetails-----------------------------------------------------*/
template <typename Order>
OrderDetails<Order>::OrderDetails(MarketMember* market_member_ptr,
                                  size_t num_units,
                                  OrderType::PaymentCurrencyType unit_price,
                                  TradingSide side)
    : market_member_ptr_(market_member_ptr),
      num_units_(num_units),
      unit_price_(unit_price),
      side_(side) {
}

}  // namespace order_details

}  // namespace trading

#endif