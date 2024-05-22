#ifndef SEGMENT_IMPL_HPP
#define SEGMENT_IMPL_HPP

namespace trading {

/*-----------------------------------------------SegmentConfig-------------------------------------------------*/
template <template <typename, typename> typename Storage>
SegmentConfig<Storage>::SegmentConfig(boost::asio::io_context& context,
                                      Milliseconds analyze_period)
    : context(context),
      analyze_period(analyze_period) {
}

/*--------------------------------------------------Segment----------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
template <template <typename, typename> typename Storage>
Segment<TargetCurrency, PaymentCurrency>::Segment(
    const SegmentConfig<Storage>& config)
    : storage_ptr_(std::make_unique<Storage<OrderType, DealType>>()),
      segment_analyzer_ptr_(std::make_shared<SegmentAnalyzerType>(
          config.context, config.analyze_period)) {
  segment_analyzer_ptr_->HandlerStart(std::bind(&Segment::Analyze, this));
}

template <typename TargetCurrency, typename PaymentCurrency>
Segment<TargetCurrency, PaymentCurrency>::Segment(Segment&& other) {
  other.segment_analyzer_ptr_->HandlerStop();

  storage_ptr_ = std::move(other.storage_ptr_);
  segment_analyzer_ptr_ = std::move(other.segment_analyzer_ptr_);

  segment_analyzer_ptr_->HandlerStart(std::bind(&Segment::Analyze, this));
}

template <typename TargetCurrency, typename PaymentCurrency>
Segment<TargetCurrency, PaymentCurrency>&
Segment<TargetCurrency, PaymentCurrency>::operator=(Segment&& other) {
  if (this != &other) {
    other.segment_analyzer_ptr_->HandlerStop();

    storage_ptr_ = std::move(other.storage_ptr_);
    segment_analyzer_ptr_ = std::move(other.segment_analyzer_ptr_);

    segment_analyzer_ptr_->HandlerStart(std::bind(&Segment::Analyze, this));
  }
  return *this;
}

template <typename TargetCurrency, typename PaymentCurrency>
Segment<TargetCurrency, PaymentCurrency>::~Segment() {
  segment_analyzer_ptr_.reset();
}

template <typename TargetCurrency, typename PaymentCurrency>
void Segment<TargetCurrency, PaymentCurrency>::AddOrder(OrderType order) {
  storage_ptr_->AddOrder(std::move(order));
}

template <typename TargetCurrency, typename PaymentCurrency>
std::optional<typename Segment<TargetCurrency, PaymentCurrency>::OrderType>
Segment<TargetCurrency, PaymentCurrency>::GetOrder(OrderType::ID id) const {
  return storage_ptr_->GetOrder(std::move(id));
}

template <typename TargetCurrency, typename PaymentCurrency>
void Segment<TargetCurrency, PaymentCurrency>::AddDeal(DealType deal) {
  storage_ptr_->AddDeal(deal);
}

template <typename TargetCurrency, typename PaymentCurrency>
std::optional<typename Segment<TargetCurrency, PaymentCurrency>::DealType>
Segment<TargetCurrency, PaymentCurrency>::GetDeal(DealType::ID id) const {
  return storage_ptr_->GetDeal(std::move(id));
}

template <typename TargetCurrency, typename PaymentCurrency>
void Segment<TargetCurrency, PaymentCurrency>::Analyze() {
  storage_ptr_->FindMatchingOrders();
}

}  // namespace trading

#endif