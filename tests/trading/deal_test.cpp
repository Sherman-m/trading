#include <gtest/gtest.h>

#include "../../src/trading/market_member.hpp"
#include "../../src/trading/order.hpp"
#include "../../src/trading/deal.hpp"
#include "../../src/trading/currency.hpp"

namespace deal_test {

using namespace trading;
using namespace order_details;
using namespace currencies;
/*------------------------------------------------------------common--------------------------------------------------------------*/

class CommonForDealTest : public testing::Test {
 protected:
  using TargetCurrency = Usd;
  using PaymentCurrency = Rubles;

  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using OrdersMatchingType = OrderType::MatchingType;
  using OrdersMatchingResultType = OrdersMatchingType::Result;

  using DealType = Deal<TargetCurrency, PaymentCurrency>;

  MarketMember mm_1_{0};
  MarketMember mm_2_{1};
};

/*------------------------------------------------------------sale----------------------------------------------------------------*/

class SaleOrderDetails : virtual public CommonForDealTest {
 protected:
  OrderType::ID id_{0};
  size_t num_units_{50};
  PaymentCurrency unit_price_{90};
  OrderType::Side side_{OrderType::Side::kSale};
  OrderType::DetailsType details_{&mm_1_, num_units_, unit_price_, side_};
};

class SaleOrder : public SaleOrderDetails {
 protected:
  OrderType sale_order_{id_, details_};
};

/*-------------------------------------------------------------buy----------------------------------------------------------------*/

class BuyOrderDetails : virtual public CommonForDealTest {
 protected:
  OrderType::ID id_{1};
  size_t num_units_{30};
  PaymentCurrency unit_price_{92};
  OrderType::Side side_{OrderType::Side::kBuy};
  OrderType::DetailsType details_{&mm_2_, num_units_, unit_price_, side_};
};

class BuyOrder : public BuyOrderDetails {
 protected:
  OrderType buy_order_{id_, details_};
};

/*------------------------------------------------------------tests---------------------------------------------------------------*/

class DealFromOrdersMatchingResult : public SaleOrder,
                                     public BuyOrder {
 protected:
  DealType::ID id_{0};
  size_t diff_ = std::min(buy_order_.Details().NumUnits(),
                          sale_order_.Details().NumUnits());
  PaymentCurrency paid_ = diff_ * buy_order_.Details().UnitPrice();

  void CheckingBuyerPart(const DealType::PartType& buyer_part) {
    ASSERT_EQ(buyer_part.MarketMemberPtr(),
              buy_order_.Details().MarketMemberPtr());
    ASSERT_EQ(buyer_part.OrderId(), buy_order_.Id());
    ASSERT_EQ(buyer_part.NumUnits(), diff_);
    ASSERT_EQ(buyer_part.Paid(), paid_);
    ASSERT_EQ(buyer_part.Side(), DealType::Side::kBuy);
  }

  void CheckingSellerPart(const DealType::PartType& seller_part) {
    ASSERT_EQ(seller_part.MarketMemberPtr(),
              sale_order_.Details().MarketMemberPtr());
    ASSERT_EQ(seller_part.OrderId(), sale_order_.Id());
    ASSERT_EQ(seller_part.NumUnits(), diff_);
    ASSERT_EQ(seller_part.Paid(), paid_);
    ASSERT_EQ(seller_part.Side(), DealType::Side::kSale);
  }
};

TEST_F(DealFromOrdersMatchingResult, CheckingDealConstruction) {
  DealType deal(id_, OrdersMatchingType::Match(sale_order_, buy_order_));
  ASSERT_EQ(deal.Id(), id_);

  auto& [buyer_part, seller_part] = deal.BuyerSellerParts();
  CheckingBuyerPart(buyer_part);
  CheckingSellerPart(seller_part);
}

}  // namespace deal_test