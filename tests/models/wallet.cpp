#include <gtest/gtest.h>

#include <trading/models/wallet.hpp>
#include <tuple>

TRADING_NAMESPACE_BEGIN

namespace {

using namespace models;
using namespace currencies;

/*------------------------------------------------tests--------------------------------------------------------*/
class WalletByDefault : public testing::Test {
 protected:
  Wallet wallet_;
};

TEST_F(WalletByDefault, WalletIsEmptyByDefault) {
  auto currency = std::get<0>(wallet_.TotalBalance());
  ASSERT_EQ(currency, decltype(currency)(0));
}

TEST_F(WalletByDefault, WalletAccuringByAmountCurrency) {
  Usd u(5);
  wallet_.TopUp(u);
  wallet_.TopUp(Usd(10));
  ASSERT_EQ(wallet_.CurrencyBalance<Usd>(), Usd(15));
}

TEST_F(WalletByDefault, WalletAccuringByNumUnits) {
  wallet_.TopUp<Usd>(10);
  ASSERT_EQ(wallet_.CurrencyBalance<Usd>(), Usd(10));
}

TEST_F(WalletByDefault, WalletWithdrawByAmountCurrency) {
  Usd u(5);
  wallet_.Withdraw(u);
  wallet_.Withdraw(Usd(10));
  ASSERT_EQ(wallet_.CurrencyBalance<Usd>(), -Usd(15));
}

TEST_F(WalletByDefault, WalletWithdrawByNumUnits) {
  wallet_.Withdraw<Usd>(10);
  ASSERT_EQ(wallet_.CurrencyBalance<Usd>(), -Usd(10));
}

}  // namespace

TRADING_NAMESPACE_END