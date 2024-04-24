#include <gtest/gtest.h>

#include "../../src/trading/market_member.hpp"
#include "../../src/trading/order.hpp"
#include "../../src/trading/currency.hpp"

using namespace trading;
using namespace order_details;
using namespace currencies;

class CommonForOrderTest : public testing::Test {
 protected:
  using TargetCurrency = Usd;
  using PaymentCurrency = Rubles;
  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using OrdersMatchingType = OrderType::OrdersMatchingType;
  using OrdersMatchingResultType = OrderType::OrdersMatchingResultType;

  MarketMember mm_1_{1};
  MarketMember mm_2_{2};
};

class SaleOrderDetailsMM1 : virtual public CommonForOrderTest {
 protected:
  OrderType::Id id_{0};
  size_t num_units_{50};
  PaymentCurrency unit_price_{90};
  OrderType::Side side_{OrderType::Side::kSale};
  OrderType::DetailsType details_{&mm_1_, num_units_, unit_price_, side_};
};

class BuyOrderDetailsMM1 : virtual public CommonForOrderTest {
 protected:
  OrderType::Id id_{0};
  size_t num_units_{50};
  PaymentCurrency unit_price_{90};
  OrderType::Side side_{OrderType::Side::kBuy};
  OrderType::DetailsType details_{&mm_1_, num_units_, unit_price_, side_};
};

class SaleOrderMM1 : public SaleOrderDetailsMM1 {
 protected:
  OrderType sale_order_{id_, details_};
};

class BuyOrderMM1 : public BuyOrderDetailsMM1 {
 protected:
  OrderType buy_order_{id_, details_};
};

class SaleOrderDetailsMM2 : virtual public CommonForOrderTest {
 protected:
  OrderType::Id id_{1};
  size_t num_units_{30};
  PaymentCurrency unit_price_{92};
  OrderType::Side side_{OrderType::Side::kSale};
  OrderType::DetailsType details_{&mm_2_, num_units_, unit_price_, side_};
};

class BuyOrderDetailsMM2 : virtual public CommonForOrderTest {
 protected:
  OrderType::Id id_{1};
  size_t num_units_{30};
  PaymentCurrency unit_price_{92};
  OrderType::Side side_{OrderType::Side::kBuy};
  OrderType::DetailsType details_{&mm_2_, num_units_, unit_price_, side_};
};

class SaleOrderMM2 : public SaleOrderDetailsMM2 {
 protected:
  OrderType sale_order_{id_, details_};
};

class BuyOrderMM2 : public BuyOrderDetailsMM2 {
 protected:
  OrderType buy_order_{id_, details_};
};

class TwoOrdersWithSameSide : public SaleOrderMM1,
                              public SaleOrderMM2 {};

TEST_F(TwoOrdersWithSameSide, OrdersWithSameSideDoNotMatch) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(SaleOrderMM1::sale_order_,
                                                 SaleOrderMM2::sale_order_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(SaleOrderMM1::sale_order_,
                                               SaleOrderMM2::sale_order_),
               std::runtime_error);
}

class TwoOrdersOneMember : public SaleOrderMM1,
                           public BuyOrderMM1 {};

TEST_F(TwoOrdersOneMember, OrdersOneMemberDoesNotMatch) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_),
               std::runtime_error);
}

class TwoNonMatchingOrdersByNumUnits : public SaleOrderMM1,
                                       public BuyOrderMM2 {
 protected:
  void SetUp() override {
    buy_order_.GetDetails().num_units += sale_order_.GetDetails().num_units;
  }
};

TEST_F(TwoNonMatchingOrdersByNumUnits, OrdersDoNotMatchInNumUnits) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_),
               std::runtime_error);
}

class TwoNonMatchingOrdersByUnitPrice : public SaleOrderMM1,
                                        public BuyOrderMM2 {
 protected:
  void SetUp() override {
    buy_order_.GetDetails().unit_price =
        sale_order_.GetDetails().unit_price / 4;
  }
};

TEST_F(TwoNonMatchingOrdersByUnitPrice, OrdersDoNotMatchInUnitPrice) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_),
               std::runtime_error);
}

class TwoMatchingOrders : public SaleOrderMM1,
                          public BuyOrderMM2 {};

TEST_F(TwoMatchingOrders, CheckingMatchingOrders) {
  ASSERT_TRUE(OrdersMatchingType::DoOrdersMatch(buy_order_, sale_order_));
  ASSERT_TRUE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));

  ASSERT_NO_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_));
}

class OrdersMatchingResult : public TwoMatchingOrders {
 protected:
  size_t diff_ = std::min(buy_order_.GetDetails().num_units,
                          sale_order_.GetDetails().num_units);
  PaymentCurrency paid_ = diff_ * buy_order_.GetDetails().unit_price;

  void Checking(const OrdersMatchingResultType& result) {
    ASSERT_EQ(result.BuyerPtr(), buy_order_.GetDetails().market_member_ptr);
    ASSERT_EQ(result.SellerPtr(), sale_order_.GetDetails().market_member_ptr);

    ASSERT_EQ(result.BuyerOrderId(), buy_order_.GetId());
    ASSERT_EQ(result.SellerOrderId(), sale_order_.GetId());

    ASSERT_EQ(result.Diff(), diff_);
    ASSERT_EQ(result.Paid(), paid_);

    ASSERT_TRUE(buy_order_.IsCompleted());
    ASSERT_FALSE(sale_order_.IsCompleted());
  }
};

TEST_F(OrdersMatchingResult, IncorrectOrdersMatchingResultConstruction) {
  ASSERT_THROW(OrdersMatchingResultType(sale_order_, sale_order_),
               std::runtime_error);
}

TEST_F(OrdersMatchingResult, CorrectOrdersMatchingResultConstruction) {
  ASSERT_NO_THROW(OrdersMatchingResultType(sale_order_, buy_order_));
}

TEST_F(OrdersMatchingResult, CheckingOrdersMatchingResultConstruction) {
  Checking(OrdersMatchingResultType(sale_order_, buy_order_));
}

TEST_F(OrdersMatchingResult, CheckingOrdersMatchingResult) {
  Checking(OrdersMatchingType::MatchOrders(sale_order_, buy_order_));
}