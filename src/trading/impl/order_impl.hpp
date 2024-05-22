#ifndef ORDER_IMPL_HPP
#define ORDER_IMPL_HPP

namespace trading {

template <typename TargetCurrency, typename PaymentCurrency>
Order<TargetCurrency, PaymentCurrency>::Order(ID id, DetailsType details)
    : id_(id), details_(std::move(details)) {}

}  // namespace trading

#endif