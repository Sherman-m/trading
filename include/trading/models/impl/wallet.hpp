#ifndef WALLET_IMPL_HPP
#define WALLET_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename Currency,
          std::enable_if_t<!std::is_arithmetic_v<Currency>, bool>>
void Wallet::TopUp(const Currency& amount) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance += amount;
}

template <typename Currency>
void Wallet::TopUp(size_t num_units,
                   const std::type_identity_t<Currency>& currency_type) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance += Currency(num_units);
}

template <typename Currency,
          std::enable_if_t<!std::is_arithmetic_v<Currency>, bool>>
void Wallet::Withdraw(const Currency& amount) {
  auto& currency_balance = std::get<Currency>(balance_);
  currency_balance -= amount;
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