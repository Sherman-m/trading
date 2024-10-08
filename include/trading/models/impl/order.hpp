#ifndef ORDER_IMPL_HPP
#define ORDER_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

template <typename TargetCurrency, typename PaymentCurrency>
Order<TargetCurrency, PaymentCurrency>::Order(ID id, DetailsType details)
    : id_(id),
      details_(std::move(details)) {
}

}  // namespace models

TRADING_NAMESPACE_END

#endif