#ifndef MARKET_MEMBER_HPP
#define MARKET_MEMBER_HPP

#include <cinttypes>
#include <type_traits>
#include <unordered_set>
#include "wallet.hpp"
#include "order_details.hpp"

namespace trading {

class MarketMember {
 public:
  using Id = uint32_t;

  using OrderId = order_details::OrderBase::Id;
  using OrderSide = order_details::OrderSide;
  template <typename TargetCurrency, typename PaymentCurrency>
  using OrderDetails =
      order_details::OrderDetails<TargetCurrency, PaymentCurrency>;

  // TODO: after implementation Deal class
  // using DealId = deal_details::DealBase::Id;
  // using DealSide = deal_details::DealSide;

  // template <typename TargetCurrency, typename PaymentCurrency>
  // using DealPart = deal_details::DealPart<TargetCurrency, PaymentCurrency>;

 public:
  MarketMember(Id id)
      : id_(id) {
  }

  Id GetId() const noexcept {
    return id_;
  }

  const Wallet::BalanceType& GetTotalBalance() const noexcept {
    return wallet_.GetTotalBalance();
  }

  template <typename Currency>
  const Currency& GetCurrencyBalance() const {
    return wallet_.GetCurrencyBalance<Currency>();
  }

  template <typename... Currencies, typename... Args>
  void TopUpWallet(const Args&... args) {
    TopUp<Currencies...>(args...);
  }

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  OrderId BuyCurrency(size_t num_units, PaymentCurrency unit_price) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(num_units, unit_price,
                                                      OrderSide::kBuy);
  }

  template <typename TargetCurrency, typename PaymentCurrency>
  OrderId BuyCurrency(size_t num_units, size_t unit_price,
                      std::type_identity_t<PaymentCurrency> payment_currency =
                          PaymentCurrency()) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(
        num_units, PaymentCurrency(unit_price), OrderSide::kBuy);
  }

  template <
      typename TargetCurrency, typename PaymentCurrency,
      std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool> = true>
  OrderId SaleCurrency(size_t num_units, PaymentCurrency unit_price) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(num_units, unit_price,
                                                      OrderSide::kSale);
  }

  template <typename TargetCurrency, typename PaymentCurrency>
  OrderId SaleCurrency(size_t num_units, size_t unit_price,
                       std::type_identity_t<PaymentCurrency> payment_currency =
                           PaymentCurrency()) {
    return MakeOrder<TargetCurrency, PaymentCurrency>(
        num_units, PaymentCurrency(unit_price), OrderSide::kSale);
  }

  template <typename TargetCurrency, typename PaymentCurrency>
  void CompleteOrder(
      const OrderDetails<TargetCurrency, PaymentCurrency>& order_details) {
  }

 private:
  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(const Currency& currency, const Args&... args) {
    wallet_.Accure(currency);
    TopUp<Currencies...>(args...);
  }

  template <typename Currency, typename... Currencies, typename... Args>
  void TopUp(size_t num_currency_units, const Args&... args) {
    wallet_.Accure<Currency>(num_currency_units);
    TopUp<Currencies...>(args...);
  }

  template <typename Currency>
  void TopUp(const Currency& currency) {
    wallet_.Accure(currency);
  }

  template <typename Currency>
  void TopUp(size_t num_currency_units) {
    wallet_.Accure<Currency>(num_currency_units);
  }

  /* TODO: after implementation Deal class
  template <typename TargetCurrency, typename PaymentCurrency>
  CloseDealPart(DealId deal_id, const DealPart<TargetCurrency, PaymentCurrency>&
  deal_part);
   */

  /* TODO: after implementation TradingPlatform class
  template <typename TargetCurrency, typename PaymentCurrency>
  OrderId MakeOrder(size_t num_units, PaymentCurrency unit_price,
                    OrderSide order_side) {
    OrderDetails<TargetCurrency, PaymentCurrency> order_details(
      this, num_units, unit_price, order_side);
    OrderId order_id =
    Trading::Get()->PublishOrder(std::move(order_details));
    active_orders_id_.push_back(order_id);
    return order_id;
  }*/

 private:
  Id id_;
  Wallet wallet_;
};

}  // namespace trading

#endif