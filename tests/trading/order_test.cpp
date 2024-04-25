#include <gtest/gtest.h>

#include "../../src/trading/market_member.hpp"
#include "../../src/trading/order.hpp"
#include "../../src/trading/currency.hpp"

namespace order_test {

using namespace trading;
using namespace order_details;
using namespace currencies;

/*------------------------------------------------------------Common--------------------------------------------------------------*/

class CommonForOrderTest : public testing::Test {
 protected:
  using TargetCurrency = Usd;
  using PaymentCurrency = Rubles;

  using OrderType = Order<TargetCurrency, PaymentCurrency>;

  using OrdersMatchingType = OrderType::MatchingType;
  using OrdersMatchingResultType = OrdersMatchingType::Result;

  MarketMember mm_1_{1};
  MarketMember mm_2_{2};

  const size_t num_units_50_{50};
  const size_t num_units_30_{30};

  const PaymentCurrency unit_price_90_{90};
  const PaymentCurrency unit_price_30_{30};

  OrderType::Side sale_side_{OrderType::Side::kSale};
  OrderType::Side buy_side_{OrderType::Side::kBuy};

  static OrderType::ID order_id;
};

CommonForOrderTest::OrderType::ID CommonForOrderTest::order_id = 0;

/*------------------------------------------------------------MM1-----------------------------------------------------------------*/

class SaleOrdersMM1 : virtual public CommonForOrderTest {
 protected:
  OrderType sale_order_50_for_90_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_50_, unit_price_90_,
                                         sale_side_}};

  OrderType sale_order_50_for_30_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_50_, unit_price_30_,
                                         sale_side_}};

  OrderType sale_order_30_for_30_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_30_, unit_price_30_,
                                         sale_side_}};
};

class BuyOrdersMM1 : virtual public CommonForOrderTest {
 protected:
  OrderType buy_order_50_for_90_{
      ++order_id,
      OrderType::DetailsType{&mm_1_, num_units_50_, unit_price_90_, buy_side_}};
};

/*------------------------------------------------------------MM2-----------------------------------------------------------------*/

class BuyOrdersMM2 : virtual public CommonForOrderTest {
 protected:
  OrderType buy_order_50_for_90_{
      ++order_id,
      OrderType::DetailsType{&mm_2_, num_units_50_, unit_price_90_, buy_side_}};

  OrderType buy_order_30_for_30_{
      ++order_id,
      OrderType::DetailsType{&mm_2_, num_units_30_, unit_price_30_, buy_side_}};
};

class SaleOrdersMM2 : virtual public CommonForOrderTest {
 protected:
  OrderType sale_order_30_for_30_{
      ++order_id, OrderType::DetailsType{&mm_2_, num_units_30_, unit_price_30_,
                                         sale_side_}};
};

/*------------------------------------------------------------tests---------------------------------------------------------------*/

class TwoOrdersWithSameSide : public SaleOrdersMM1,
                              public SaleOrdersMM2 {};

TEST_F(TwoOrdersWithSameSide, OrdersWithSameSideDoNotMatch) {
  ASSERT_FALSE(
      OrdersMatchingType::DoMatch(SaleOrdersMM1::sale_order_30_for_30_,
                                  SaleOrdersMM2::sale_order_30_for_30_));
  ASSERT_THROW(OrdersMatchingType::Match(SaleOrdersMM1::sale_order_30_for_30_,
                                         SaleOrdersMM2::sale_order_30_for_30_),
               std::runtime_error);
}

class TwoOrdersOneMember : public SaleOrdersMM1,
                           public BuyOrdersMM1 {};

TEST_F(TwoOrdersOneMember, OrdersOneMemberDoesNotMatch) {
  ASSERT_FALSE(
      OrdersMatchingType::DoMatch(sale_order_50_for_90_, buy_order_50_for_90_));
  ASSERT_THROW(
      OrdersMatchingType::Match(sale_order_50_for_90_, buy_order_50_for_90_),
      std::runtime_error);
}

class TwoNonMatchingOrdersByUnitPrice : public SaleOrdersMM1,
                                        public BuyOrdersMM2 {};

TEST_F(TwoNonMatchingOrdersByUnitPrice, OrdersDoNotMatchInUnitPrice) {
  ASSERT_FALSE(
      OrdersMatchingType::DoMatch(sale_order_50_for_90_, buy_order_30_for_30_));
  ASSERT_THROW(
      OrdersMatchingType::Match(sale_order_50_for_90_, buy_order_30_for_30_),
      std::runtime_error);
}

class TwoMatchingOrders : public SaleOrdersMM1,
                          public BuyOrdersMM2 {};

TEST_F(TwoMatchingOrders, CheckingMatchingOrders) {
  ASSERT_TRUE(
      OrdersMatchingType::DoMatch(buy_order_50_for_90_, sale_order_50_for_90_));
  ASSERT_TRUE(
      OrdersMatchingType::DoMatch(sale_order_50_for_90_, buy_order_50_for_90_));

  ASSERT_NO_THROW(
      OrdersMatchingType::Match(sale_order_50_for_90_, buy_order_50_for_90_));
}

class OrdersMatchingResult : public TwoMatchingOrders {
 protected:
  void Checking(OrderType& buy_order, OrderType& sale_order) {
    if (buy_order.Details().Side() == OrderType::Side::kSale &&
        sale_order.Details().Side() == OrderType::Side::kBuy) {
      return Checking(sale_order, buy_order);
    }
    auto [diff, paid] = CalcDiffPaid(buy_order, sale_order);
    auto matching_result = OrdersMatchingType::Match(sale_order, buy_order);

    ASSERT_EQ(matching_result.BuyerPtr(),
              buy_order.Details().MarketMemberPtr());
    ASSERT_EQ(matching_result.SellerPtr(),
              sale_order.Details().MarketMemberPtr());

    ASSERT_EQ(matching_result.BuyerOrderId(), buy_order.Id());
    ASSERT_EQ(matching_result.SellerOrderId(), sale_order.Id());

    ASSERT_EQ(matching_result.Diff(), diff);
    ASSERT_EQ(matching_result.Paid(), paid);
  }

 private:
  std::pair<size_t, PaymentCurrency> CalcDiffPaid(const OrderType& buy_order,
                                                  const OrderType& sale_order) {
    size_t diff = std::min(buy_order.Details().NumUnits(),
                           sale_order.Details().NumUnits());
    PaymentCurrency paid = diff * buy_order.Details().UnitPrice();

    return {diff, paid};
  }
};

TEST_F(OrdersMatchingResult, IncorrectOrdersMatchingResultConstruction) {
  ASSERT_THROW(
      OrdersMatchingResultType(sale_order_50_for_90_, sale_order_50_for_90_),
      std::runtime_error);
}

TEST_F(OrdersMatchingResult, CorrectOrdersMatchingResultConstruction) {
  ASSERT_NO_THROW(
      OrdersMatchingResultType(sale_order_30_for_30_, buy_order_50_for_90_));
}

TEST_F(OrdersMatchingResult, CheckingPartlyMatching) {
  Checking(sale_order_30_for_30_, buy_order_50_for_90_);

  ASSERT_TRUE(sale_order_30_for_30_.IsCompleted());
  ASSERT_FALSE(buy_order_50_for_90_.IsCompleted());
}

TEST_F(OrdersMatchingResult, CheckingFullMatching) {
  Checking(sale_order_30_for_30_, buy_order_30_for_30_);

  ASSERT_TRUE(sale_order_30_for_30_.IsCompleted());
  ASSERT_TRUE(buy_order_30_for_30_.IsCompleted());
}

}  // namespace order_test