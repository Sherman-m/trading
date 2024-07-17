#ifndef WALLET_HPP
#define WALLET_HPP

#include <tuple>
#include <type_traits>

#include "currency.hpp"

namespace trading {

class Wallet {
 public:
  using BalanceType = currencies::TupleCurrencyTypes;

  Wallet() = default;

  const BalanceType& TotalBalance() const noexcept {
    return balance_;
  }

  template <typename Currency>
  const Currency& CurrencyBalance() const {
    return std::get<Currency>(balance_);
  }

  template <typename Currency,
            std::enable_if_t<!std::is_arithmetic_v<Currency>, bool> = true>
  void TopUp(const Currency& amount);

  template <typename Currency>
  void TopUp(std::size_t num_units,
             const std::type_identity_t<Currency>& currency_type = Currency());

  template <typename Currency,
            std::enable_if_t<!std::is_arithmetic_v<Currency>, bool> = true>
  void Withdraw(const Currency& amount);

  template <typename Currency>
  void Withdraw(
      std::size_t num_units,
      const std::type_identity_t<Currency>& currency_type = Currency());

 private:
  BalanceType balance_;
};
}  // namespace trading

#include "impl/wallet_impl.hpp"

#endif