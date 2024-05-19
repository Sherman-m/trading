#ifndef ORDER_HPP
#define ORDER_HPP

#include <cinttypes>
#include <stdexcept>
#include <algorithm>
#include "order_details.hpp"
#include "segment_fwd.hpp"

namespace trading {

template <typename TargetCurrency, typename PaymentCurrency>
class OrdersMatching;

template <typename TargetCurrency, typename PaymentCurrency>
class Order : public order_details::OrderBase {
  friend class OrdersMatching<TargetCurrency, PaymentCurrency>;

 public:
  using ID = order_details::OrderBase::ID;
  using Side = order_details::OrderSide;
  using DetailsType =
      order_details::OrderDetails<TargetCurrency, PaymentCurrency>;

  using MatchingType = OrdersMatching<TargetCurrency, PaymentCurrency>;

  using SegmentType = Segment<TargetCurrency, PaymentCurrency>;

 public:
  explicit Order(ID id, DetailsType details)
      : id_(id),
        details_(std::move(details)) {
  }

  ID Id() const noexcept {
    return id_;
  }

  const DetailsType& Details() const noexcept {
    return details_;
  }

  DetailsType& Details() noexcept {
    return details_;
  }

  bool IsCompleted() const noexcept {
    return details_.NumUnits() == 0;
  }

  friend bool operator==(const Order& first, const Order& second) {
    return first.id_ == second.id_ && first.details_ == second.details_;
  }

  friend auto operator<=>(const Order&, const Order&) = default;

 private:
  ID id_;
  DetailsType details_;
};

template <typename TargetCurrency, typename PaymentCurrency>
class OrdersMatching {
 public:
  using OrderType = Order<TargetCurrency, PaymentCurrency>;

  class Result {
   public:
    explicit Result(OrderType& buy_order, OrderType& sale_order) {
      if (!DoMatch(buy_order, sale_order)) {
        throw std::runtime_error("Do not matching orders");
      }

      if (buy_order.details_.side_ == OrderType::Side::kSale &&
          sale_order.details_.side_ == OrderType::Side::kBuy) {
        Init(sale_order, buy_order);
      } else {
        Init(buy_order, sale_order);
      }
    }

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

    size_t Diff() const noexcept {
      return diff_;
    }

    PaymentCurrency Paid() const noexcept {
      return paid_;
    }

    friend auto operator<=>(const Result&, const Result&) = default;

   private:
    void Init(OrderType& buy_order, OrderType& sale_order) {
      buyer_seller_ptrs_ = {buy_order.details_.market_member_ptr_,
                            sale_order.details_.market_member_ptr_};
      buyer_seller_orders_id_ = {buy_order.id_, sale_order.id_};

      diff_ = std::min(buy_order.details_.num_units_,
                       sale_order.details_.num_units_);
      paid_ = diff_ * buy_order.details_.unit_price_;

      buy_order.details_.num_units_ -= diff_;
      sale_order.details_.num_units_ -= diff_;
    }

   private:
    using BuyerSellerPtrs = std::pair<MarketMember*, MarketMember*>;
    using BuyerSellerOrdersID =
        std::pair<typename OrderType::ID, typename OrderType::ID>;

    BuyerSellerPtrs buyer_seller_ptrs_;
    BuyerSellerOrdersID buyer_seller_orders_id_;
    size_t diff_;
    PaymentCurrency paid_;
  };

  static bool DoMatch(const OrderType& buy_order, const OrderType& sale_order) {
    if (buy_order.details_.side_ == OrderType::Side::kSale &&
        sale_order.details_.side_ == OrderType::Side::kBuy) {
      return DoMatch(sale_order, buy_order);
    }
    return (buy_order.details_.market_member_ptr_ !=
            sale_order.details_.market_member_ptr_) &&
           (buy_order.details_.side_ != sale_order.details_.side_) &&
           (buy_order.details_.unit_price_ >= sale_order.details_.unit_price_);
  }

  static Result Match(OrderType& buy_order, OrderType& sale_order) {
    if (buy_order.details_.side_ == OrderType::Side::kSale &&
        sale_order.details_.side_ == OrderType::Side::kBuy) {
      return Match(sale_order, buy_order);
    }

    if (!DoMatch(buy_order, sale_order)) {
      throw std::runtime_error("Orders don't match");
    }

    return Result(buy_order, sale_order);
  }
};

}  // namespace trading

#endif