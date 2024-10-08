#include <gtest/gtest.h>

#include <trading/models/market_member.hpp>
#include <trading/models/wallet.hpp>
#include <tuple>

TRADING_NAMESPACE_BEGIN

namespace {

using namespace models;
using namespace currencies;

/*-----------------------------------------------tests---------------------------------------------------------*/
class MarketMemberByDefault : public testing::Test {
 protected:
  MarketMember market_member_{0};
};

TEST_F(MarketMemberByDefault, GettingMarketMemberId) {
  ASSERT_EQ(market_member_.Id(), 0);
}

class MarketMemberWithEmptyBalance : public MarketMemberByDefault {
 protected:
  Wallet wallet_;
};

TEST_F(MarketMemberWithEmptyBalance, CheckingEmptyMarketMemberBalance) {
  ASSERT_EQ(market_member_.TotalBalance(), wallet_.TotalBalance());
}

class MarketMemberWithNotEmptyBalance : public MarketMemberWithEmptyBalance {
 protected:
  void SetUp() override {
    wallet_.TopUp<Usd>(10);
    wallet_.TopUp<Rubles>(15);
  }
};

TEST_F(MarketMemberWithNotEmptyBalance,
       TopUpMarketMemberBalanceWithCurrencyArgs) {
  market_member_.TopUpWallet(Usd(10), Rubles(15));
  ASSERT_EQ(wallet_.TotalBalance(), market_member_.TotalBalance());
}

TEST_F(MarketMemberWithNotEmptyBalance,
       TopUpMarketMemberBalanceWithArithmeticArgs) {
  market_member_.TopUpWallet<Rubles, Usd>(15, 10);
  ASSERT_EQ(wallet_.TotalBalance(), market_member_.TotalBalance());
}

TEST_F(MarketMemberWithNotEmptyBalance, TopUpMarketMemberBalanceWithMixedArgs) {
  market_member_.TopUpWallet<Rubles>(15, Usd(10));
  ASSERT_EQ(wallet_.TotalBalance(), market_member_.TotalBalance());
}

}  // namespace

TRADING_NAMESPACE_END