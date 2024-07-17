#ifndef DEAL_IMPL_HPP
#define DEAL_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename Order>
Deal<Order>::Deal(ID id, OrderType::MatchingType::Result result)
    : id_(id),
      buyer_seller_parts_(
          PartType(result.BuyerPtr(), result.BuyerOrderId(), result.Diff(),
                   result.Paid(), TradingSide::kBuy),
          PartType(result.SellerPtr(), result.SellerOrderId(), result.Diff(),
                   result.Paid(), TradingSide::kSale)) {
}

}  // namespace models

TRADING_NAMESPACE_END

#endif