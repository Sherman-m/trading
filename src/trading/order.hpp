#ifndef ORDER_HPP
#define ORDER_HPP

#include <cinttypes>
#include <stdexcept>
#include <algorithm>
#include "order_details.hpp"

namespace trading {

template <typename TargetCurrency, typename PaymentCurrency>
class OrdersMatching;

template <typename TargetCurrency, typename PaymentCurrency>
class Order : public order_details::OrderBase {
  friend class OrdersMatching<TargetCurrency, PaymentCurrency>;

 public:
  using Id = order_details::OrderBase::Id;
  using Side = order_details::OrderSide;
  using DetailsType =
      order_details::OrderDetails<TargetCurrency, PaymentCurrency>;

  using OrdersMatchingType = OrdersMatching<TargetCurrency, PaymentCurrency>;
  using OrdersMatchingResultType = OrdersMatchingType::Result;

  // TODO: after implementation Segment class
  // using SegmentType = Segment<TargetCurrency, PaymentCurrency>;

 public:
  explicit Order(Id id, DetailsType details_)
      : id_(id),
        details_(std::move(details_)) {
  }

  Id GetId() const noexcept {
    return id_;
  }

  const DetailsType& GetDetails() const noexcept {
    return details_;
  }

  DetailsType& GetDetails() noexcept {
    return details_;
  }

  bool IsCompleted() const noexcept {
    return details_.num_units == 0;
  }

 private:
  Id id_;
  DetailsType details_;
};

template <typename TargetCurrency, typename PaymentCurrency>
class OrdersMatching {
 public:
  using OrderType = Order<TargetCurrency, PaymentCurrency>;

  class Result {
   public:
    explicit Result(OrderType& buy_order, OrderType& sale_order) {
      if (!DoOrdersMatch(buy_order, sale_order)) {
        throw std::runtime_error("Do not matching orders");
      }

      if (buy_order.details_.side == OrderType::Side::kSale &&
          sale_order.details_.side == OrderType::Side::kBuy) {
        Init(sale_order, buy_order);
      } else {
        Init(buy_order, sale_order);
      }
    }

    bool operator==(const Result& other) const noexcept {
      return diff_ == other.diff_ && paid_ == other.paid_;
    }

    MarketMember* BuyerPtr() const noexcept {
      return buyer_seller_ptrs_.first;
    }

    MarketMember* SellerPtr() const noexcept {
      return buyer_seller_ptrs_.second;
    }

    OrderType::Id BuyerOrderId() const noexcept {
      return buyer_seller_orders_id_.first;
    }

    OrderType::Id SellerOrderId() const noexcept {
      return buyer_seller_orders_id_.second;
    }

    size_t Diff() const noexcept {
      return diff_;
    }

    PaymentCurrency Paid() const noexcept {
      return paid_;
    }

   private:
    void Init(OrderType& buy_order, OrderType& sale_order) {
      buyer_seller_ptrs_ = {buy_order.details_.market_member_ptr,
                            sale_order.details_.market_member_ptr};
      buyer_seller_orders_id_ = {buy_order.id_, sale_order.id_};

      diff_ =
          std::min(buy_order.details_.num_units, sale_order.details_.num_units);
      paid_ = diff_ * buy_order.details_.unit_price;

      buy_order.details_.num_units -= diff_;
      sale_order.details_.num_units -= diff_;
    }

   private:
    using BuyerSellerPtrs = std::pair<MarketMember*, MarketMember*>;
    using BuyerSellerOrdersId =
        std::pair<typename OrderType::Id, typename OrderType::Id>;

    BuyerSellerPtrs buyer_seller_ptrs_;
    BuyerSellerOrdersId buyer_seller_orders_id_;
    size_t diff_;
    PaymentCurrency paid_;
  };

  static bool DoOrdersMatch(const OrderType& buy_order,
                            const OrderType& sale_order) {
    if (buy_order.details_.side == OrderType::Side::kSale &&
        sale_order.details_.side == OrderType::Side::kBuy) {
      return DoOrdersMatch(sale_order, buy_order);
    }
    return (buy_order.details_.side != sale_order.details_.side) &&
           (buy_order.details_.num_units <= sale_order.details_.num_units) &&
           (buy_order.details_.unit_price >= sale_order.details_.unit_price);
  }

  static const Result MatchOrders(OrderType& buy_order, OrderType& sale_order) {
    if (buy_order.details_.side == OrderType::Side::kSale &&
        sale_order.details_.side == OrderType::Side::kBuy) {
      return MatchOrders(sale_order, buy_order);
    }

    if (!DoOrdersMatch(buy_order, sale_order)) {
      throw std::runtime_error("Orders don't match");
    }

    return Result(buy_order, sale_order);
  }
};

}  // namespace trading

#endif