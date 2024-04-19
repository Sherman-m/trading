#ifndef DEAL_HPP
#define DEAL_HPP

#include "deal_details.hpp"
#include "order.hpp"

namespace trading {

template <typename TargetCurrency, typename PaymentCurrency>
class Deal {
 public:
  using Id = deal_details::DealBase::Id;
  using Side = deal_details::DealSide;
  using PartType = deal_details::DealPart<TargetCurrency, PaymentCurrency>;
  using PartsType = std::pair<PartType, PartType>;

  using OrderType = Order<TargetCurrency, PaymentCurrency>;

  // TODO: add after implementation Segment class
  // using SegmentType = Segment<TargetCurrency, PaymentCurrency>;

 public:
  explicit Deal(Id id, const OrderType::OrdersMatchingResultType& result)
      : id_(id),
        buyer_part_(result.BuyerPtr(), result.BuyerOrderId(), result.Diff(),
                    result.Paid(), Side::kBuy),
        seller_part_(result.SellerPtr(), result.SellerOrderId(), result.Diff(),
                     result.Paid(), Side::kSale) {
  }

  Id GetId() const noexcept {
    return id_;
  }
  const PartType& GetBuyerPart() const noexcept {
    return buyer_part_;
  }

  const PartType& GetSellerPart() const noexcept {
    return seller_part_;
  }

  const PartsType GetBuyerSellerParts() const noexcept {
    return {buyer_part_, seller_part_};
  }

 private:
  Id id_;
  PartType buyer_part_;
  PartType seller_part_;
};
}  // namespace trading

#endif