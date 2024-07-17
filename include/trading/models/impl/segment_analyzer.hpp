#ifndef SEGMENT_ANALYZER_IMPL_HPP
#define SEGMENT_ANALYZER_IMPL_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

namespace segment_details {

template <typename SegmentType>
SegmentAnalyzer<SegmentType>::SegmentAnalyzer(net::io_context& ioc,
                                              Milliseconds period)
    : timer_(ioc),
      period_(period),
      handler_(std::bind(&SegmentAnalyzer::DefaultHandler, this)) {
}

template <typename SegmentType>
void SegmentAnalyzer<SegmentType>::HandlerStart(HandlerType handler) {
  handler_ = std::move(handler);
  last_tick_ = Clock::now();
  ScheduleTick();
}

template <typename SegmentType>
void SegmentAnalyzer<SegmentType>::HandlerStop() {
  handler_ = std::bind(&SegmentAnalyzer::DefaultHandler, this);
}

template <typename SegmentType>
void SegmentAnalyzer<SegmentType>::ScheduleTick() {
  timer_.expires_after(period_);
  timer_.async_wait([self = this->shared_from_this()](sys::error_code ec) {
    self->OnTick(ec);
  });
}

template <typename SegmentType>
void SegmentAnalyzer<SegmentType>::OnTick(sys::error_code ec) {
  if (!ec) {
    last_tick_ = Clock::now();
    handler_();
    ScheduleTick();
  }
}

template <typename SegmentType>
void SegmentAnalyzer<SegmentType>::DefaultHandler() {
  // Do nothing
}

}  // namespace segment_details

}  // namespace models

TRADING_NAMESPACE_END

#endif