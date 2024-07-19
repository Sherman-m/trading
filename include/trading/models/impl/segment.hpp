#ifndef SEGMENT_IMPL_HPP
#define SEGMENT_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

/*--------------------------------------------------Segment----------------------------------------------------*/
template <typename Config, typename TargetCurrency, typename PaymentCurrency>
Segment<Config, TargetCurrency, PaymentCurrency>::Segment(Config& config)
    : storage_ptr_(std::make_unique<StorageType>()),
      segment_analyzer_ptr_(std::make_shared<SegmentAnalyzerType>(
          config.config_.ioc_, config.config_.analyze_period_)) {
  segment_analyzer_ptr_->HandlerStart(std::bind(&Segment::Analyze, this));
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
Segment<Config, TargetCurrency, PaymentCurrency>::Segment(Segment&& other) {
  other.segment_analyzer_ptr_->HandlerStop();

  storage_ptr_ = std::move(other.storage_ptr_);
  segment_analyzer_ptr_ = std::move(other.segment_analyzer_ptr_);

  segment_analyzer_ptr_->HandlerStart(std::bind(&Segment::Analyze, this));
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
Segment<Config, TargetCurrency, PaymentCurrency>&
Segment<Config, TargetCurrency, PaymentCurrency>::operator=(Segment&& other) {
  if (this != &other) {
    other.segment_analyzer_ptr_->HandlerStop();

    storage_ptr_ = std::move(other.storage_ptr_);
    segment_analyzer_ptr_ = std::move(other.segment_analyzer_ptr_);

    segment_analyzer_ptr_->HandlerStart(std::bind(&Segment::Analyze, this));
  }
  return *this;
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
Segment<Config, TargetCurrency, PaymentCurrency>::~Segment() {
  segment_analyzer_ptr_.reset();
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
Segment<Config, TargetCurrency, PaymentCurrency>::OrderType::ID
Segment<Config, TargetCurrency, PaymentCurrency>::AddOrder(
    OrderType::DetailsType order_details) {
  return storage_ptr_->AddOrder(std::move(order_details));
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
std::optional<
    typename Segment<Config, TargetCurrency, PaymentCurrency>::OrderType>
Segment<Config, TargetCurrency, PaymentCurrency>::GetOrder(
    OrderType::ID id) const {
  return storage_ptr_->GetOrder(std::move(id));
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
Segment<Config, TargetCurrency, PaymentCurrency>::DealType::ID
Segment<Config, TargetCurrency, PaymentCurrency>::AddDeal(
    DealType::PartsType deal_parts) {
  storage_ptr_->AddDeal(deal_parts);
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
std::optional<
    typename Segment<Config, TargetCurrency, PaymentCurrency>::DealType>
Segment<Config, TargetCurrency, PaymentCurrency>::GetDeal(
    DealType::ID id) const {
  return storage_ptr_->GetDeal(std::move(id));
}

template <typename Config, typename TargetCurrency, typename PaymentCurrency>
void Segment<Config, TargetCurrency, PaymentCurrency>::Analyze() {
  storage_ptr_->FindMatchingOrders();
}

}  // namespace models

TRADING_NAMESPACE_END

#endif