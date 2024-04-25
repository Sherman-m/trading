#ifndef MARKET_MEMBER_HPP
#define MARKET_MEMBER_HPP

#include <cinttypes>
#include <type_traits>
#include "wallet.hpp"
#include "order_details.hpp"
#include "deal_details.hpp"

namespace trading {

class MarketMember {
 public:
  using ID = uint32_t;

  using OrderID = order_details::OrderBase::ID;
  using OrderSide = order_details::OrderSide;

  template <typename TargetCurrency, typename PaymentCurrency>
  using OrderDetails =
      order_details::OrderDetails<TargetCurrency, PaymentCurrency>;

  using DealID = deal_details::DealBase::ID;
  using DealSide = deal_details::DealSide;

  template <typename TargetCurrency, typename PaymentCurrency>
  using DealPart = deal_details::DealPart<TargetCurrency, PaymentCurrency>;

 public:
  MarketMember(ID id)
      : id_(id) {
  }

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
  void TopUpWallet(const Args&... args) {
    TopUp<Currencies...>(args...);
  }

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  OrderID BuyCurrency(size_t num_units, PaymentCurrency unit_price) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(num_units, unit_price,
                                                      OrderSide::kBuy);
  }

  template <typename TargetCurrency, typename PaymentCurrency>
  OrderID BuyCurrency(size_t num_units, size_t unit_price,
                      std::type_identity_t<PaymentCurrency> payment_currency =
                          PaymentCurrency()) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(
        num_units, PaymentCurrency(unit_price), OrderSide::kBuy);
  }

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  OrderID SaleCurrency(size_t num_units, PaymentCurrency unit_price) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(num_units, unit_price,
                                                      OrderSide::kSale);
  }

  template <typename TargetCurrency, typename PaymentCurrency>
  OrderID SaleCurrency(size_t num_units, size_t unit_price,
                       std::type_identity_t<PaymentCurrency> payment_currency =
                           PaymentCurrency()) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(
        num_units, PaymentCurrency(unit_price), OrderSide::kSale);
  }

  // TODO: need to think about safe interaction with wallet
  template <typename TargetCurrency, typename PaymentCurrency>
  void CloseDealPart(
      const DealPart<TargetCurrency, PaymentCurrency>& deal_part) {
    if (deal_part.Side() == deal_details::DealSide::kBuy) {
      wallet_.Withdraw<PaymentCurrency>(deal_part.Paid());
      wallet_.TopUp<TargetCurrency>(deal_part.NumUnits());
    } else if (deal_part.Side() == deal_details::DealSide::kSale) {
      wallet_.TopUp<PaymentCurrency>(deal_part.Paid());
      wallet_.Withdraw<TargetCurrency>(deal_part.NumUnits());
    }
  }

 private:
  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(const Currency& currency, const Args&... args) {
    wallet_.TopUp(currency);
    TopUp<Currencies...>(args...);
  }

  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(size_t num_currency_units, const Args&... args) {
    wallet_.TopUp<Currency>(num_currency_units);
    TopUp<Currencies...>(args...);
  }

  template <typename Currency>
  void TopUp(const Currency& currency) {
    wallet_.TopUp(currency);
  }

  template <typename Currency>
  void TopUp(size_t num_currency_units) {
    wallet_.TopUp<Currency>(num_currency_units);
  }

  // TODO: after implementation TardingPlatform
  template <typename TargetCurrency, typename PaymentCurrency>
  OrderID MakeOrder(size_t num_units, PaymentCurrency unit_price,
                    OrderSide order_side) {
    OrderDetails<TargetCurrency, PaymentCurrency> order_details(
        this, num_units, unit_price, order_side);
    // OrderId order_id =
    // Trading::Get()->PublishOrder(std::move(order_details));
    return /*order_id*/ 0;
  }

 private:
  ID id_;
  Wallet wallet_;
};

}  // namespace trading

#endif