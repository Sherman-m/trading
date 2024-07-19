#ifndef MARKET_MEMBER_IMPL_HPP
#define MARKET_MEMBER_IMPL_HPP

#include <trading/models/deal.hpp>
#include <trading/models/order.hpp>

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename Config>
MarketMember<Config>::MarketMember(ID id)
    : id_(id) {
}

template <typename Config>
template <typename... Currencies, typename... Args>
void MarketMember<Config>::TopUpWallet(const Args&... args) {
  wallet_.TopUp<Currencies...>(args...);
}

template <typename Config>
template <typename... Currencies, typename... Args>
void MarketMember<Config>::WithdrawWallet(const Args&... args) {
  wallet_.Withdraw<Currencies...>(args...);
}

template <typename Config>
template <typename TargetCurrency, typename PaymentCurrency,
          std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool>>
typename MarketMember<Config>::OrderType<TargetCurrency, PaymentCurrency>::ID
MarketMember<Config>::BuyCurrency(size_t num_units,
                                  PaymentCurrency unit_price) {
  return MakeOrder<TargetCurrency, PaymentCurrency>(num_units, unit_price,
                                                    TradingSide::kBuy);
}

template <typename Config>
template <typename TargetCurrency, typename PaymentCurrency>
typename MarketMember<Config>::OrderType<TargetCurrency, PaymentCurrency>::ID
MarketMember<Config>::BuyCurrency(
    size_t num_units, size_t unit_price,
    std::type_identity_t<PaymentCurrency> payment_currency) {
  return MakeOrder<TargetCurrency, PaymentCurrency>(
      num_units, PaymentCurrency(unit_price), TradingSide::kBuy);
}

template <typename Config>
template <typename TargetCurrency, typename PaymentCurrency,
          std::enable_if_t<!std::is_arithmetic_v<PaymentCurrency>, bool>>
typename MarketMember<Config>::OrderType<TargetCurrency, PaymentCurrency>::ID
MarketMember<Config>::SaleCurrency(size_t num_units,
                                   PaymentCurrency unit_price) {
  return MakeOrder<TargetCurrency, PaymentCurrency>(num_units, unit_price,
                                                    TradingSide::kSale);
}

template <typename Config>
template <typename TargetCurrency, typename PaymentCurrency>
typename MarketMember<Config>::OrderType<TargetCurrency, PaymentCurrency>::ID
MarketMember<Config>::SaleCurrency(
    size_t num_units, size_t unit_price,
    std::type_identity_t<PaymentCurrency> payment_currency) {
  return MakeOrder<TargetCurrency, PaymentCurrency>(
      num_units, PaymentCurrency(unit_price), TradingSide::kSale);
}

// TODO: need to think about safe interaction with wallet
template <typename Config>
template <typename Deal>
void MarketMember<Config>::CloseDealPart(const Deal::PartType& deal_part) {
  if (deal_part.Side() == TradingSide::kBuy) {
    wallet_.Withdraw<typename Deal::PaymentCurrencyType>(deal_part.Paid());
    wallet_.TopUp<typename Deal::TargetCurrencyType>(deal_part.NumUnits());
  } else if (deal_part.Side() == TradingSide::kSale) {
    wallet_.TopUp<typename Deal::PaymentCurrencyType>(deal_part.Paid());
    wallet_.Withdraw<typename Deal::TargetCurrencyType>(deal_part.NumUnits());
  }
}

template <typename Config>
template <typename TargetCurrency, typename PaymentCurrency>
typename MarketMember<Config>::OrderType<TargetCurrency, PaymentCurrency>::ID
MarketMember<Config>::MakeOrder(size_t num_units, PaymentCurrency unit_price,
                                TradingSide trading_side) {
  using OrderType = OrderType<TargetCurrency, PaymentCurrency>;

  typename OrderType::DetailsType order_details(id_, num_units, unit_price,
                                                trading_side);

  return Config::TradingPlatformScopeType::Get()
      ->template PublishOrder<OrderType>(std::move(order_details));
}

}  // namespace models

TRADING_NAMESPACE_END

#endif