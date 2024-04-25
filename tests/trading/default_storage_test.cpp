#include <gtest/gtest.h>
#include <cinttypes>

#include "../../src/trading/market_member.hpp"
#include "../../src/trading/default_storage.hpp"
#include "../../src/trading/currency.hpp"

namespace default_storage_test {

using namespace trading;
using namespace currencies;
using namespace storages;

/*------------------------------------------------------------common--------------------------------------------------------------*/

class CommonForDefaultStorageTest : public testing::Test {
 protected:
  using TargetCurrency = Usd;
  using PaymentCurrency = Rubles;

  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using DealType = Deal<TargetCurrency, PaymentCurrency>;

  using DefaultStorageType = DefaultStorage<TargetCurrency, PaymentCurrency>;

  MarketMember mm_1_{0};
  MarketMember mm_2_{1};

  const size_t num_units_30_{30};
  const size_t num_units_50_{50};

  const PaymentCurrency unit_price_40_{40};
  const PaymentCurrency unit_price_50_{50};
  const PaymentCurrency unit_price_100_{100};

  OrderType::Side buy_side_{OrderType::Side::kBuy};
  OrderType::Side sale_side_{OrderType::Side::kSale};

  static OrderType::ID order_id;
};

CommonForDefaultStorageTest::OrderType::ID
    CommonForDefaultStorageTest::order_id = 0;

/*------------------------------------------------------------MM1-----------------------------------------------------------------*/

class SaleOrdersMM1 : virtual public CommonForDefaultStorageTest {
 protected:
  OrderType sale_order_30_for_50_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_30_, unit_price_50_,
                                         sale_side_}};

  OrderType sale_order_50_for_40_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_50_, unit_price_40_,
                                         sale_side_}};

  OrderType sale_order_50_for_100_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_30_, unit_price_100_,
                                         sale_side_}};

  OrderType sale_order_50_for_50_{
      ++order_id, OrderType::DetailsType{&mm_1_, num_units_50_, unit_price_50_,
                                         sale_side_}};
};

/*------------------------------------------------------------MM2-----------------------------------------------------------------*/

class BuyOrdersMM2 : virtual public CommonForDefaultStorageTest {
 protected:
  OrderType buy_order_30_for_50_{
      ++order_id,
      OrderType::DetailsType{&mm_2_, num_units_30_, unit_price_50_, buy_side_}};

  OrderType buy_order_50_for_40_{
      ++order_id,
      OrderType::DetailsType{&mm_2_, num_units_50_, unit_price_40_, buy_side_}};

  OrderType buy_order_50_for_50_{
      ++order_id,
      OrderType::DetailsType{&mm_2_, num_units_50_, unit_price_50_, buy_side_}};
};

/*------------------------------------------------------------tests---------------------------------------------------------------*/

class DefaultStorageTestOrders : public SaleOrdersMM1,
                                 public BuyOrdersMM2 {
 protected:
  DefaultStorageType storage_;
};

TEST_F(DefaultStorageTestOrders, CheckingAddingTwoOrders) {
  storage_.AddOrder(sale_order_30_for_50_);
  storage_.AddOrder(buy_order_30_for_50_);

  ASSERT_TRUE(storage_.GetOrder(sale_order_30_for_50_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(sale_order_30_for_50_.Id()),
            sale_order_30_for_50_);

  ASSERT_TRUE(storage_.GetOrder(buy_order_30_for_50_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(buy_order_30_for_50_.Id()), buy_order_30_for_50_);
}

TEST_F(DefaultStorageTestOrders, AddingTwoMatchingOrdersWillBeDeleted) {
  storage_.AddOrder(sale_order_30_for_50_);
  storage_.AddOrder(buy_order_30_for_50_);

  storage_.FindMatchingOrders();

  ASSERT_FALSE(storage_.GetOrder(sale_order_30_for_50_.Id()).has_value());
  ASSERT_FALSE(storage_.GetOrder(buy_order_30_for_50_.Id()).has_value());
}

TEST_F(DefaultStorageTestOrders, AddingTwoMatchingOrdersWhereOneWillBeDeleted) {
  storage_.AddOrder(sale_order_50_for_40_);
  storage_.AddOrder(buy_order_30_for_50_);

  storage_.FindMatchingOrders();

  ASSERT_TRUE(storage_.GetOrder(sale_order_50_for_40_.Id()).has_value());
  ASSERT_NE(storage_.GetOrder(sale_order_30_for_50_.Id()),
            sale_order_30_for_50_);

  ASSERT_FALSE(storage_.GetOrder(buy_order_30_for_50_.Id()).has_value());
}

TEST_F(DefaultStorageTestOrders, AddingTwoNonMatchingOrdersWillNotBeDeleted) {
  storage_.AddOrder(sale_order_50_for_100_);
  storage_.AddOrder(buy_order_50_for_40_);

  storage_.FindMatchingOrders();

  ASSERT_TRUE(storage_.GetOrder(sale_order_50_for_100_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(sale_order_50_for_100_.Id()),
            sale_order_50_for_100_);

  ASSERT_TRUE(storage_.GetOrder(buy_order_50_for_40_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(buy_order_50_for_40_.Id()), buy_order_50_for_40_);
}

TEST_F(DefaultStorageTestOrders, OrdersWithMaxUnitPriceForBuyOrdersMatchFirst) {
  storage_.AddOrder(buy_order_50_for_40_);
  storage_.AddOrder(buy_order_30_for_50_);
  storage_.AddOrder(sale_order_50_for_40_);

  storage_.FindMatchingOrders();

  ASSERT_FALSE(storage_.GetOrder(sale_order_50_for_40_.Id()).has_value());

  ASSERT_FALSE(storage_.GetOrder(buy_order_30_for_50_.Id()).has_value());

  size_t expected_remains = buy_order_50_for_40_.Details().NumUnits() -
                            (sale_order_50_for_40_.Details().NumUnits() -
                             buy_order_30_for_50_.Details().NumUnits());

  ASSERT_TRUE(storage_.GetOrder(buy_order_50_for_40_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(buy_order_50_for_40_.Id())->Details().NumUnits(),
            expected_remains);
}

TEST_F(DefaultStorageTestOrders,
       OrdersWithMinUnitPriceForSaleOrdersMatchFirst) {
  storage_.AddOrder(sale_order_30_for_50_);
  storage_.AddOrder(sale_order_50_for_40_);
  storage_.AddOrder(buy_order_30_for_50_);

  storage_.FindMatchingOrders();

  ASSERT_FALSE(storage_.GetOrder(buy_order_30_for_50_.Id()).has_value());

  ASSERT_TRUE(storage_.GetOrder(sale_order_30_for_50_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(sale_order_30_for_50_.Id()),
            sale_order_30_for_50_);

  size_t expected_remains = sale_order_50_for_40_.Details().NumUnits() -
                            buy_order_30_for_50_.Details().NumUnits();

  ASSERT_TRUE(storage_.GetOrder(sale_order_50_for_40_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(sale_order_50_for_40_.Id())->Details().NumUnits(),
            expected_remains);
}

TEST_F(DefaultStorageTestOrders, MultipleOrders) {
  storage_.AddOrder(sale_order_30_for_50_);   // 3) removed
  storage_.AddOrder(sale_order_50_for_40_);   // 1) 20 remains; 2) removed
  storage_.AddOrder(sale_order_50_for_100_);  // 5) not match
  storage_.AddOrder(sale_order_50_for_50_);   // 4) not match

  storage_.AddOrder(buy_order_30_for_50_);  // 1) removed
  storage_.AddOrder(buy_order_50_for_40_);  // 4) not match; 5) not match
  storage_.AddOrder(buy_order_50_for_50_);  // 2) 30 remains; 3) removed

  storage_.FindMatchingOrders();

  ASSERT_FALSE(storage_.GetOrder(buy_order_30_for_50_.Id()).has_value());
  ASSERT_FALSE(storage_.GetOrder(sale_order_50_for_40_.Id()).has_value());
  ASSERT_FALSE(storage_.GetOrder(buy_order_50_for_50_.Id()).has_value());
  ASSERT_FALSE(storage_.GetOrder(sale_order_30_for_50_.Id()).has_value());

  ASSERT_TRUE(storage_.GetOrder(buy_order_50_for_40_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(buy_order_50_for_40_.Id()), buy_order_50_for_40_);

  ASSERT_TRUE(storage_.GetOrder(sale_order_50_for_100_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(sale_order_50_for_100_.Id()),
            sale_order_50_for_100_);

  ASSERT_TRUE(storage_.GetOrder(sale_order_50_for_50_.Id()).has_value());
  ASSERT_EQ(storage_.GetOrder(sale_order_50_for_50_.Id()),
            sale_order_50_for_50_);
}
//  TODO: redesign tests for Deals after implementation TradingPlatform
//    1. TradingPlatform generates deal and adds this to Segment storage
//    2. Need to add a field about Deal to fields of test classes and compare
//    it with deal stored into Storage

}  // namespace default_storage_test