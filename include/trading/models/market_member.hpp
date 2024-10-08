#ifndef MARKET_MEMBER_HPP
#define MARKET_MEMBER_HPP

#include <cstdint>
#include <trading/core/trading_platform.hpp>
#include <trading/models/deal_fwd.hpp>
#include <trading/models/order_fwd.hpp>
#include <trading/models/trading_side.hpp>
#include <trading/models/wallet.hpp>
#include <type_traits>

TRADING_NAMESPACE_BEGIN

namespace models {

class MarketMember {
 public:
  using ID = std::uint32_t;

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
      std::size_t num_units, PaymentCurrency unit_price);

  template <typename TargetCurrency, typename PaymentCurrency>
  Order<TargetCurrency, PaymentCurrency>::ID BuyCurrency(
      std::size_t num_units, std::size_t unit_price,
      std::type_identity_t<PaymentCurrency> payment_currency =
          PaymentCurrency());

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  Order<TargetCurrency, PaymentCurrency>::ID SaleCurrency(
      std::size_t num_units, PaymentCurrency unit_price);

  template <typename TargetCurrency, typename PaymentCurrency>
  Order<TargetCurrency, PaymentCurrency>::ID SaleCurrency(
      std::size_t num_units, std::size_t unit_price,
      std::type_identity_t<PaymentCurrency> payment_currency =
          PaymentCurrency());

  template <typename Deal>
  void CloseDealPart(const Deal::PartType& deal_part);

 private:
  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(const Currency& currency, const Args&... args);

  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(std::size_t num_currency_units, const Args&... args);

  template <typename Currency>
  void TopUp(const Currency& currency);

  template <typename Currency>
  void TopUp(std::size_t num_currency_units);

  template <typename Order>
  Order::ID MakeOrder(std::size_t num_units,
                      Order::PaymentCurrencyType unit_price,
                      TradingSide trading_side);

  ID id_;
  Wallet wallet_;
};

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/market_member.hpp>

#endif