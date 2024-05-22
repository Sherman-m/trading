#ifndef DEAL_HPP
#define DEAL_HPP

#include "deal_details.hpp"
#include "order_fwd.hpp"
#include "orders_matching_fwd.hpp"
#include "segment_fwd.hpp"

namespace trading {

template <typename Order>
class Deal : public deal_details::DealBase {
 public:
  using BaseType = deal_details::DealBase;
  using ID = BaseType::ID;

  using OrderType = Order;

  using TargetCurrencyType = OrderType::TargetCurrencyType;
  using PaymentCurrencyType = OrderType::PaymentCurrencyType;

  using PartType = deal_details::DealPart<Deal>;
  using PartsType = std::pair<PartType, PartType>;

  using SegmentType = Order::SegmentType;

 public:
  explicit Deal(ID id, OrderType::MatchingType::Result result);

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

#include "impl/deal_impl.hpp"

#endif