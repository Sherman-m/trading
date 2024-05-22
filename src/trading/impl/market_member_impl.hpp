#ifndef MARKET_MEMBER_IMPL_HPP
#define MARKET_MEMBER_IMPL_HPP

#include "../deal.hpp"
#include "../order.hpp"

namespace trading {

template <typename... Currencies, typename... Args>
void MarketMember::TopUpWallet(const Args&... args) {
  TopUp<Currencies...>(args...);
}

template <typename TargetCurrency, typename PaymentCurrency,
          std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool>>
Order<TargetCurrency, PaymentCurrency>::ID MarketMember::BuyCurrency(
    size_t num_units, PaymentCurrency unit_price) {
  return MakeOrder<Order<TargetCurrency, PaymentCurrency>>(
      num_units, unit_price, TradingSide::kBuy);
}

template <typename TargetCurrency, typename PaymentCurrency>
Order<TargetCurrency, PaymentCurrency>::ID MarketMember::BuyCurrency(
    size_t num_units, size_t unit_price,
    std::type_identity_t<PaymentCurrency> payment_currency) {
  return MakeOrder<Order<TargetCurrency, PaymentCurrency>>(
      num_units, PaymentCurrency(unit_price), TradingSide::kBuy);
}

template <typename TargetCurrency, typename PaymentCurrency,
          std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool>>
Order<TargetCurrency, PaymentCurrency>::ID MarketMember::SaleCurrency(
    size_t num_units, PaymentCurrency unit_price) {
  return MakeOrder<Order<TargetCurrency, PaymentCurrency>>(
      num_units, unit_price, TradingSide::kSale);
}

template <typename TargetCurrency, typename PaymentCurrency>
Order<TargetCurrency, PaymentCurrency>::ID MarketMember::SaleCurrency(
    size_t num_units, size_t unit_price,
    std::type_identity_t<PaymentCurrency> payment_currency) {
  return MakeOrder<Order<TargetCurrency, PaymentCurrency>>(
      num_units, PaymentCurrency(unit_price), TradingSide::kSale);
}

// TODO: need to think about safe interaction with wallet
template <typename Deal>
void MarketMember::CloseDealPart(const Deal::PartType& deal_part) {
  if (deal_part.Side() == TradingSide::kBuy) {
    wallet_.Withdraw<typename Deal::PaymentCurrencyType>(deal_part.Paid());
    wallet_.TopUp<typename Deal::TargetCurrencyType>(deal_part.NumUnits());
  } else if (deal_part.Side() == TradingSide::kSale) {
    wallet_.TopUp<typename Deal::PaymentCurrencyType>(deal_part.Paid());
    wallet_.Withdraw<typename Deal::TargetCurrencyType>(deal_part.NumUnits());
  }
}

template <typename Currency, typename... Currencies, typename... Args>
void MarketMember::TopUp(const Currency& currency, const Args&... args) {
  wallet_.TopUp(currency);
  TopUp<Currencies...>(args...);
}

template <typename Currency, typename... Currencies, typename... Args>
void MarketMember::TopUp(size_t num_currency_units, const Args&... args) {
  wallet_.TopUp<Currency>(num_currency_units);
  TopUp<Currencies...>(args...);
}

template <typename Currency>
void MarketMember::TopUp(const Currency& currency) {
  wallet_.TopUp(currency);
}

template <typename Currency>
void MarketMember::TopUp(size_t num_currency_units) {
  wallet_.TopUp<Currency>(num_currency_units);
}

template <typename Order>
Order::ID MarketMember::MakeOrder(size_t num_units,
                                  Order::PaymentCurrencyType unit_price,
                                  TradingSide trading_side) {
  typename Order::DetailsType order_details(this, num_units, unit_price,
                                            trading_side);
  return TradingPlatformScope::Get()->PublishOrder<Order>(
      std::move(order_details));
}

}  // namespace trading

#endif