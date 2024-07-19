#ifndef ORDERS_MATCHING_HPP
#define ORDERS_MATCHING_HPP

#include <trading/models/market_member_fwd.hpp>
#include <utility>

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename Order>
class OrdersMatching {
 public:
  using OrderType = Order;
  using MarketMember = OrderType::MarketMemberType;

  /*----------------------------------------OrdersMatching::Result-----------------------------------------------*/
  class Result {
   public:
    explicit Result(OrderType& buy_order, OrderType& sale_order);

    const MarketMember::ID BuyerId() const noexcept {
      return buyer_seller_id_.first;
    }

    MarketMember::ID BuyerId() noexcept {
      return buyer_seller_id_.first;
    }

    const MarketMember::ID SellerId() const noexcept {
      return buyer_seller_id_.second;
    }

    MarketMember::ID SellerId() noexcept {
      return buyer_seller_id_.second;
    }

    OrderType::ID BuyerOrderId() const noexcept {
      return buyer_seller_orders_id_.first;
    }

    OrderType::ID SellerOrderId() const noexcept {
      return buyer_seller_orders_id_.second;
    }

    std::size_t Diff() const noexcept {
      return diff_;
    }

    OrderType::PaymentCurrencyType Paid() const noexcept {
      return paid_;
    }

    friend auto operator<=>(const Result&, const Result&) = default;

   private:
    void Init(OrderType& buy_order, OrderType& sale_order);

    using BuyerSellerID =
        std::pair<typename MarketMember::ID, typename MarketMember::ID>;
    using BuyerSellerOrdersID =
        std::pair<typename OrderType::ID, typename OrderType::ID>;

    BuyerSellerID buyer_seller_id_;
    BuyerSellerOrdersID buyer_seller_orders_id_;
    std::size_t diff_;
    OrderType::PaymentCurrencyType paid_;
  };

  /*-------------------------------------------------------------------------------------------------------------*/

  static bool DoMatch(const OrderType& buy_order, const OrderType& sale_order);

  static Result Match(OrderType& buy_order, OrderType& sale_order);
};

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/orders_matching.hpp>

#endif