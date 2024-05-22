#ifndef ORDERS_MATCHING_IMPL_HPP
#define ORDERS_MATCHING_IMPL_HPP

#include <stdexcept>

namespace trading {

/*----------------------------------------OrdersMatching-------------------------------------------------------*/
/*----------------------------------------OrdersMatching::Result-----------------------------------------------*/
template <typename Order>
OrdersMatching<Order>::Result::Result(OrderType& buy_order,
                                      OrderType& sale_order) {
  if (!DoMatch(buy_order, sale_order)) {
    throw std::runtime_error("Do not matching orders");
  }

  if (buy_order.details_.side_ == TradingSide::kSale &&
      sale_order.details_.side_ == TradingSide::kBuy) {
    Init(sale_order, buy_order);
  } else {
    Init(buy_order, sale_order);
  }
}

template <typename Order>
void OrdersMatching<Order>::Result::Init(OrderType& buy_order,
                                         OrderType& sale_order) {
  buyer_seller_ptrs_ = {buy_order.details_.market_member_ptr_,
                        sale_order.details_.market_member_ptr_};
  buyer_seller_orders_id_ = {buy_order.id_, sale_order.id_};

  diff_ =
      std::min(buy_order.details_.num_units_, sale_order.details_.num_units_);
  paid_ = diff_ * buy_order.details_.unit_price_;

  buy_order.details_.num_units_ -= diff_;
  sale_order.details_.num_units_ -= diff_;
}
/*-------------------------------------------------------------------------------------------------------------*/

template <typename Order>
bool OrdersMatching<Order>::DoMatch(const OrderType& buy_order,
                                    const OrderType& sale_order) {
  if (buy_order.details_.side_ == TradingSide::kSale &&
      sale_order.details_.side_ == TradingSide::kBuy) {
    return DoMatch(sale_order, buy_order);
  }
  return (buy_order.details_.market_member_ptr_ !=
          sale_order.details_.market_member_ptr_) &&
         (buy_order.details_.side_ != sale_order.details_.side_) &&
         (buy_order.details_.unit_price_ >= sale_order.details_.unit_price_);
}

template <typename Order>
OrdersMatching<Order>::Result OrdersMatching<Order>::Match(
    OrderType& buy_order, OrderType& sale_order) {
  if (buy_order.details_.side_ == TradingSide::kSale &&
      sale_order.details_.side_ == TradingSide::kBuy) {
    return Match(sale_order, buy_order);
  }

  if (!DoMatch(buy_order, sale_order)) {
    throw std::runtime_error("Orders don't match");
  }

  return Result(buy_order, sale_order);
}

}  // namespace trading

#endif