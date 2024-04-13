#include <gtest/gtest.h>

#include <tuple>
#include "../../src/trading/market_member.hpp"
#include "../../src/trading/wallet.hpp"

using namespace trading;
using namespace currencies;

class MarketMemberByDefault : public testing::Test {
 protected:
  MarketMember market_member_{0};
};

TEST_F(MarketMemberByDefault, GettingMarketMemberId) {
  ASSERT_EQ(market_member_.GetId(), 0);
}

class MarketMemberWithEmptyBalance : public MarketMemberByDefault {
 protected:
  Wallet wallet_;
};

TEST_F(MarketMemberWithEmptyBalance, CheckingEmptyMarketMemberBalance) {
  ASSERT_EQ(market_member_.GetTotalBalance(), wallet_.GetTotalBalance());
}

class MarketMemberWithNotEmptyBalance : public MarketMemberWithEmptyBalance {
 protected:
  void SetUp() override {
    wallet_.Accure<Usd>(10);
    wallet_.Accure<Rubles>(15);
  }
};

TEST_F(MarketMemberWithNotEmptyBalance,
       TopUpMarketMemberBalanceWithCurrencyArgs) {
  market_member_.TopUpWallet(Usd(10), Rubles(15));
  ASSERT_EQ(wallet_.GetTotalBalance(), market_member_.GetTotalBalance());
}

TEST_F(MarketMemberWithNotEmptyBalance,
       TopUpMarketMemberBalanceWithArithmeticArgs) {
  market_member_.TopUpWallet<Rubles, Usd>(15, 10);
  ASSERT_EQ(wallet_.GetTotalBalance(), market_member_.GetTotalBalance());
}

TEST_F(MarketMemberWithNotEmptyBalance, TopUpMarketMemberBalanceWithMixedArgs) {
  market_member_.TopUpWallet<Rubles>(15, Usd(10));
  ASSERT_EQ(wallet_.GetTotalBalance(), market_member_.GetTotalBalance());
}