#ifndef WALLET_IMPL_HPP
#define WALLET_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename Currency, typename... Currencies, typename... Args>
void Wallet::TopUp(const Currency& amount, const Args&... args) {
  TopUp(amount);
  TopUp<Currencies...>(args...);
}

template <typename Currency,
          std::enable_if_t<!std::is_arithmetic_v<Currency>, bool>>
void Wallet::TopUp(const Currency& amount) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance += amount;
}

template <typename Currency, typename... Currencies, typename... Args>
void Wallet::TopUp(size_t num_units, const Args&... args) {
  TopUp<Currency>(num_units);
  TopUp<Currencies...>(args...);
}

template <typename Currency>
void Wallet::TopUp(size_t num_units,
                   const std::type_identity_t<Currency>& currency_type) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance += Currency(num_units);
}

template <typename Currency, typename... Currencies, typename... Args>
void Wallet::Withdraw(const Currency& amount, const Args&... args) {
  Withdraw(amount);
  Withdraw<Currencies...>(args...);
}

template <typename Currency,
          std::enable_if_t<!std::is_arithmetic_v<Currency>, bool>>
void Wallet::Withdraw(const Currency& amount) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance -= amount;
}

template <typename Currency, typename... Currencies, typename... Args>
void Wallet::Withdraw(size_t num_units, const Args&... args) {
  Withdraw<Currency>(num_units);
  Withdraw<Currencies...>(args...);
}

template <typename Currency>
void Wallet::Withdraw(size_t num_units,
                      const std::type_identity_t<Currency>& currency_type) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance -= Currency(num_units);
}

}  // namespace models

TRADING_NAMESPACE_END

#endif