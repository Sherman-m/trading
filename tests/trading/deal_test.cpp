#include <gtest/gtest.h>

#include "../../src/trading/order.hpp"
#include "../../src/trading/deal.hpp"
#include "../../src/trading/currency.hpp"

using namespace trading;
using namespace order_details;
using namespace currencies;

class CommonForTests : public testing::Test {
 protected:
  using TargetCurrency = Usd;
  using PaymentCurrency = Rubles;

  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using OrdersMatchingType = OrderType::OrdersMatchingType;
  using OrdersMatchingResultType = OrderType::OrdersMatchingResultType;

  using DealType = Deal<TargetCurrency, PaymentCurrency>;

  MarketMember* zero_mm_ptr_{nullptr};
};

class SaleOrderDetails : virtual public CommonForTests {
 protected:
  OrderType::Id id_{0};
  size_t num_units_{50};
  PaymentCurrency unit_price_{90};
  OrderType::Side side_{OrderType::Side::kSale};
  OrderType::DetailsType details_{zero_mm_ptr_, num_units_, unit_price_, side_};
};

class SaleOrder : public SaleOrderDetails {
 protected:
  OrderType sale_order_{id_, details_};
};

class BuyOrderDetails : virtual public CommonForTests {
 protected:
  OrderType::Id id_{1};
  size_t num_units_{30};
  PaymentCurrency unit_price_{92};
  OrderType::Side side_{OrderType::Side::kBuy};
  OrderType::DetailsType details_{zero_mm_ptr_, num_units_, unit_price_, side_};
};

class BuyOrder : public BuyOrderDetails {
 protected:
  OrderType buy_order_{id_, details_};
};

class DealFromOrdersMatchingResult : public SaleOrder,
                                     public BuyOrder {
 protected:
  DealType::Id id_{0};
  size_t diff_ = std::min(buy_order_.GetOrderDetails().num_units,
                          sale_order_.GetOrderDetails().num_units);
  PaymentCurrency paid_ = diff_ * buy_order_.GetOrderDetails().unit_price;

  void CheckingBuyerPart(const DealType::PartType& buyer_part) {
    ASSERT_EQ(buyer_part.market_member_ptr_,
              buy_order_.GetOrderDetails().market_member_ptr);
    ASSERT_EQ(buyer_part.order_id, buy_order_.GetId());
    ASSERT_EQ(buyer_part.num_units, diff_);
    ASSERT_EQ(buyer_part.paid, paid_);
    ASSERT_EQ(buyer_part.side, DealType::Side::kBuy);
  }

  void CheckingSellerPart(const DealType::PartType& seller_part) {
    ASSERT_EQ(seller_part.market_member_ptr_,
              sale_order_.GetOrderDetails().market_member_ptr);
    ASSERT_EQ(seller_part.order_id, sale_order_.GetId());
    ASSERT_EQ(seller_part.num_units, diff_);
    ASSERT_EQ(seller_part.paid, paid_);
    ASSERT_EQ(seller_part.side, DealType::Side::kSale);
  }
};

TEST_F(DealFromOrdersMatchingResult, CheckingDealConstruction) {
  DealType deal(id_, OrdersMatchingType::MatchOrders(sale_order_, buy_order_));
  ASSERT_EQ(deal.GetId(), id_);

  auto& [buyer_part, seller_part] = deal.GetBuyerSellerParts();
  CheckingBuyerPart(buyer_part);
  CheckingSellerPart(seller_part);
}