#ifndef ORDERS_MATCHING_HPP
#define ORDERS_MATCHING_HPP

#include "market_member_fwd.hpp"
#include <utility>

namespace trading {

template <typename Order>
class OrdersMatching {
 public:
  using OrderType = Order;
  /*----------------------------------------OrdersMatching::Result-----------------------------------------------*/
  class Result {
   public:
    explicit Result(OrderType& buy_order, OrderType& sale_order);

    const MarketMember* BuyerPtr() const noexcept {
      return buyer_seller_ptrs_.first;
    }

    MarketMember* BuyerPtr() noexcept {
      return buyer_seller_ptrs_.first;
    }

    const MarketMember* SellerPtr() const noexcept {
      return buyer_seller_ptrs_.second;
    }

    MarketMember* SellerPtr() noexcept {
      return buyer_seller_ptrs_.second;
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

    using BuyerSellerPtrs = std::pair<MarketMember*, MarketMember*>;
    using BuyerSellerOrdersID =
        std::pair<typename OrderType::ID, typename OrderType::ID>;

    BuyerSellerPtrs buyer_seller_ptrs_;
    BuyerSellerOrdersID buyer_seller_orders_id_;
    std::size_t diff_;
    OrderType::PaymentCurrencyType paid_;
  };

  /*-------------------------------------------------------------------------------------------------------------*/

  static bool DoMatch(const OrderType& buy_order, const OrderType& sale_order);

  static Result Match(OrderType& buy_order, OrderType& sale_order);
};

}  // namespace trading

#include "impl/orders_matching_impl.hpp"

#endif