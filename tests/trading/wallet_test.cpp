#include <gtest/gtest.h>

#include "../../src/trading/wallet.hpp"
#include <tuple>

using namespace trading;
using namespace trading::currencies;

class WalletByDefault : public testing::Test {
 protected:
  Wallet wallet_;
};

TEST_F(WalletByDefault, WalletIsEmptyByDefault) {
  auto currency = std::get<0>(wallet_.GetTotalBalance());
  ASSERT_EQ(currency, decltype(currency)(0));
}

TEST_F(WalletByDefault, WalletAccuringByAmountCurrency) {
  Usd u(5);
  wallet_.Accure(u);
  wallet_.Accure(Usd(10));
  ASSERT_EQ(wallet_.GetCurrencyBalance<Usd>(), Usd(15));
}

TEST_F(WalletByDefault, WalletAccuringByNumUnits) {
  wallet_.Accure<Usd>(10);
  ASSERT_EQ(wallet_.GetCurrencyBalance<Usd>(), Usd(10));
}

TEST_F(WalletByDefault, WalletWithdrawByAmountCurrency) {
  Usd u(5);
  wallet_.Withdraw(u);
  wallet_.Withdraw(Usd(10));
  ASSERT_EQ(wallet_.GetCurrencyBalance<Usd>(), -Usd(15));
}

TEST_F(WalletByDefault, WalletWithdrawByNumUnits) {
  wallet_.Withdraw<Usd>(10);
  ASSERT_EQ(wallet_.GetCurrencyBalance<Usd>(), -Usd(10));
}