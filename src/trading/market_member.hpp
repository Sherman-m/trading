#ifndef MARKET_MEMBER_HPP
#define MARKET_MEMBER_HPP

#include <cinttypes>
#include <type_traits>

#include "deal_fwd.hpp"
#include "order_fwd.hpp"
#include "trading_platform.hpp"
#include "wallet.hpp"

namespace trading {

class MarketMember {
 public:
  using ID = uint32_t;

 public:
  MarketMember(ID id);

  ID Id() const noexcept {
    return id_;
  }

  const Wallet::BalanceType& TotalBalance() const noexcept {
    return wallet_.TotalBalance();
  }

  template <typename Currency>
  const Currency& CurrencyBalance() const {
    return wallet_.CurrencyBalance<Currency>();
  }

  template <typename... Currencies, typename... Args>
  void TopUpWallet(const Args&... args);

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  Order<TargetCurrency, PaymentCurrency>::ID BuyCurrency(
      size_t num_units, PaymentCurrency unit_price);

  template <typename TargetCurrency, typename PaymentCurrency>
  Order<TargetCurrency, PaymentCurrency>::ID BuyCurrency(
      size_t num_units, size_t unit_price,
      std::type_identity_t<PaymentCurrency> payment_currency =
          PaymentCurrency());

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  Order<TargetCurrency, PaymentCurrency>::ID SaleCurrency(
      size_t num_units, PaymentCurrency unit_price);

  template <typename TargetCurrency, typename PaymentCurrency>
  Order<TargetCurrency, PaymentCurrency>::ID SaleCurrency(
      size_t num_units, size_t unit_price,
      std::type_identity_t<PaymentCurrency> payment_currency =
          PaymentCurrency());

  template <typename Deal>
  void CloseDealPart(const Deal::PartType& deal_part);

 private:
  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(const Currency& currency, const Args&... args);

  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(size_t num_currency_units, const Args&... args);

  template <typename Currency>
  void TopUp(const Currency& currency);

  template <typename Currency>
  void TopUp(size_t num_currency_units);

  template <typename Order>
  Order::ID MakeOrder(size_t num_units, Order::PaymentCurrencyType unit_price,
                      TradingSide trading_side);

 private:
  ID id_;
  Wallet wallet_;
};

}  // namespace trading

#include "impl/market_member_impl.hpp"

#endif