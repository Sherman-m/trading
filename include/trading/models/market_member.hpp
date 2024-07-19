#ifndef MARKET_MEMBER_HPP
#define MARKET_MEMBER_HPP

#include <cstdint>
#include <trading/core/trading_platform.hpp>
#include <trading/models/segment_fwd.hpp>
#include <trading/models/trading_side.hpp>
#include <trading/models/wallet.hpp>
#include <type_traits>

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename Config>
class MarketMember {
 public:
  using ID = typename Config::Id;

  template <typename TargetCurrency, typename PaymentCurrency>
  using OrderType = typename Config::OrderType<TargetCurrency, PaymentCurrency>;

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

  template <typename... Currencies, typename... Args>
  void WithdrawWallet(const Args&... args);

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  OrderType<TargetCurrency, PaymentCurrency>::ID BuyCurrency(
      std::size_t num_units, PaymentCurrency unit_price);

  template <typename TargetCurrency, typename PaymentCurrency>
  OrderType<TargetCurrency, PaymentCurrency>::ID BuyCurrency(
      std::size_t num_units, std::size_t unit_price,
      std::type_identity_t<PaymentCurrency> payment_currency =
          PaymentCurrency());

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  OrderType<TargetCurrency, PaymentCurrency>::ID SaleCurrency(
      std::size_t num_units, PaymentCurrency unit_price);

  template <typename TargetCurrency, typename PaymentCurrency>
  OrderType<TargetCurrency, PaymentCurrency>::ID SaleCurrency(
      std::size_t num_units, std::size_t unit_price,
      std::type_identity_t<PaymentCurrency> payment_currency =
          PaymentCurrency());

  template <typename Deal>
  void CloseDealPart(const Deal::PartType& deal_part);

 private:
  template <typename TargetCurrency, typename PaymentCurrency>
  OrderType<TargetCurrency, PaymentCurrency>::ID MakeOrder(
      std::size_t num_units, PaymentCurrency unit_price,
      TradingSide trading_side);

  ID id_;
  Wallet wallet_;
};

}  // namespace models

TRADING_NAMESPACE_END

#include <trading/models/impl/market_member.hpp>

#endif