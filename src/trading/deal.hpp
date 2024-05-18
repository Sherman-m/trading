#ifndef DEAL_HPP
#define DEAL_HPP

#include "deal_details.hpp"
#include "order.hpp"

namespace trading {

template <typename TargetCurrency, typename PaymentCurrency>
class Deal : public deal_details::DealBase {
 public:
  using ID = deal_details::DealBase::ID;
  using Side = deal_details::DealSide;
  using PartType = deal_details::DealPart<TargetCurrency, PaymentCurrency>;
  using PartsType = std::pair<PartType, PartType>;

  using OrderType = Order<TargetCurrency, PaymentCurrency>;

  using SegmentType = Segment<TargetCurrency, PaymentCurrency>;

 public:
  explicit Deal(ID id, OrderType::MatchingType::Result result)
      : id_(id),
        buyer_seller_parts_(
            PartType(result.BuyerPtr(), result.BuyerOrderId(), result.Diff(),
                     result.Paid(), Side::kBuy),
            PartType(result.SellerPtr(), result.SellerOrderId(), result.Diff(),
                     result.Paid(), Side::kSale)) {
  }

  ID Id() const noexcept {
    return id_;
  }

  const PartType& BuyerPart() const noexcept {
    return buyer_seller_parts_.first;
  }

  PartType& BuyerPart() noexcept {
    return buyer_seller_parts_.first;
  }

  const PartType& SellerPart() const noexcept {
    return buyer_seller_parts_.second;
  }

  PartType& SellerPart() noexcept {
    return buyer_seller_parts_.second;
  }

  const PartsType& BuyerSellerParts() const noexcept {
    return buyer_seller_parts_;
  }

  PartsType& BuyerSellerParts() noexcept {
    return buyer_seller_parts_;
  }

  friend bool operator==(const Deal& first, const Deal& second) {
    return first.id_ == second.id_ && first.buyer_part_ == second.buyer_part_ &&
           first.seller_part_ == second.seller_part_;
  }

  friend auto operator<=>(const Deal& first, const Deal& second) = default;

 private:
  ID id_;
  PartsType buyer_seller_parts_;
};

}  // namespace trading

#endif