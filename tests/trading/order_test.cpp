#include <gtest/gtest.h>

#include "../../src/trading/order.hpp"
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

class TwoOrdersWithSameSide : public SaleOrder {
 protected:
  OrderType sale_order_1_{sale_order_};
};

TEST_F(TwoOrdersWithSameSide, OrdersWithSameSideDoNotMatch) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(sale_order_, sale_order_1_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(sale_order_, sale_order_1_),
               std::runtime_error);
}

class TwoNonMatchingOrdersByNumUnits : public SaleOrder,
                                       public BuyOrder {
 protected:
  void SetUp() override {
    buy_order_.GetOrderDetails().num_units +=
        sale_order_.GetOrderDetails().num_units;
  }
};

TEST_F(TwoNonMatchingOrdersByNumUnits, OrdersDoNotMatchInNumUnits) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_),
               std::runtime_error);
}

class TwoNonMatchingOrdersByUnitPrice : public SaleOrder,
                                        public BuyOrder {
 protected:
  void SetUp() override {
    buy_order_.GetOrderDetails().unit_price =
        sale_order_.GetOrderDetails().unit_price / 4;
  }
};

TEST_F(TwoNonMatchingOrdersByUnitPrice, OrdersDoNotMatchInUnitPrice) {
  ASSERT_FALSE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));
  ASSERT_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_),
               std::runtime_error);
}

class TwoMatchingOrders : public SaleOrder,
                          public BuyOrder {};

TEST_F(TwoMatchingOrders, CheckingMatchingOrders) {
  ASSERT_TRUE(OrdersMatchingType::DoOrdersMatch(buy_order_, sale_order_));
  ASSERT_TRUE(OrdersMatchingType::DoOrdersMatch(sale_order_, buy_order_));

  ASSERT_NO_THROW(OrdersMatchingType::MatchOrders(sale_order_, buy_order_));
}

class OrdersMatchingResult : public TwoMatchingOrders {
 protected:
  size_t diff_ = std::min(buy_order_.GetOrderDetails().num_units,
                          sale_order_.GetOrderDetails().num_units);
  PaymentCurrency paid_ = diff_ * buy_order_.GetOrderDetails().unit_price;

  void Checking(const OrdersMatchingResultType& result) {
    ASSERT_EQ(result.BuyerPtr(),
              buy_order_.GetOrderDetails().market_member_ptr);
    ASSERT_EQ(result.SellerPtr(),
              sale_order_.GetOrderDetails().market_member_ptr);

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