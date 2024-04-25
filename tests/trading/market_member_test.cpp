#include <gtest/gtest.h>

#include <tuple>
#include "../../src/trading/market_member.hpp"
#include "../../src/trading/wallet.hpp"

namespace market_member_test {

using namespace trading;
using namespace currencies;

/*------------------------------------------------------------tests---------------------------------------------------------------*/

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

}  // namespace market_member_test