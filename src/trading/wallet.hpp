#ifndef WALLET_HPP
#define WALLET_HPP

#include <tuple>
#include <type_traits>
#include "currency.hpp"

namespace trading {

class Wallet {
 public:
  using BalanceType = std::tuple<currency::Usd, currency::Rubles>;

 public:
  Wallet() = default;

  const BalanceType& GetTotalBalance() const noexcept {
    return balance_;
  }

  template <typename Currency>
  const Currency& GetCurrencyBalance() const {
    return std::get<Currency>(balance_);
  }

  template <typename Currency,
            std::enable_if_t<!std::is_arithmetic_v<Currency>, bool> = true>
  void Accure(const Currency& amount) {
    auto& currency_balance = std::get<Currency>(balance_);
    currency_balance += amount;
  }

  template <typename Currency>
  void Accure(
      size_t num_units,
      const std::type_identity_t<Currency>& currency_type = Currency()) {
    auto& currency_balance = std::get<Currency>(balance_);
    currency_balance += Currency(num_units);
  }

  template <typename Currency,
            std::enable_if_t<!std::is_arithmetic_v<Currency>, bool> = true>
  void Withdraw(const Currency& amount) {
    auto& currency_balance = std::get<Currency>(balance_);
    currency_balance -= amount;
  }

  template <typename Currency>
  void Withdraw(
      size_t num_units,
      const std::type_identity_t<Currency>& currency_type = Currency()) {
    auto& currency_balance = std::get<Currency>(balance_);
    currency_balance -= Currency(num_units);
  }

 private:
  BalanceType balance_;
};
}  // namespace trading

#endif