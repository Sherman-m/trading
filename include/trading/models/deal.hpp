#ifndef DEAL_HPP
#define DEAL_HPP

#include <trading/models/deal_details.hpp>
#include <trading/models/segment_fwd.hpp>
#include <utility>

TRADING_NAMESPACE_BEGIN

namespace models {

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

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/deal.hpp>

#endif