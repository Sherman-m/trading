#ifndef SEGMENT_ANALYZER_HPP
#define SEGMENT_ANALYZER_HPP

#include <boost/asio/steady_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>

#include <memory>
#include <utility>
#include <chrono>

namespace trading {

namespace segment_details {

namespace net = boost::asio;
namespace sys = boost::system;

template <typename SegmentType>
class SegmentAnalyzer
    : public std::enable_shared_from_this<SegmentAnalyzer<SegmentType>> {
 public:
  using Timer = net::steady_timer;
  using Milliseconds = std::chrono::milliseconds;
  using Clock = std::chrono::steady_clock;
  using HandlerType = std::function<void()>;

 public:
  explicit SegmentAnalyzer(net::io_context& ioc, Milliseconds period)
      : timer_(ioc),
        period_(period),
        handler_(std::bind(&SegmentAnalyzer::DefaultHandler, this)) {
  }

  SegmentAnalyzer(const SegmentAnalyzer&) = delete;
  SegmentAnalyzer& operator=(const SegmentAnalyzer&) = delete;

  SegmentAnalyzer(SegmentAnalyzer&&) = default;
  SegmentAnalyzer& operator=(SegmentAnalyzer&&) = default;

  void Start(HandlerType handler) {
    handler_ = std::move(handler);
    last_tick_ = Clock::now();
    ScheduleTick();
  }

  void Stop() {
    handler_ = std::bind(&SegmentAnalyzer::DefaultHandler, this);
  }

 private:
  void ScheduleTick() {
    timer_.expires_after(period_);
    timer_.async_wait([self = this->shared_from_this()](sys::error_code ec) {
      self->OnTick(ec);
    });
  }

  void OnTick(sys::error_code ec) {
    if (!ec) {
      last_tick_ = Clock::now();
      handler_();
      ScheduleTick();
    }
  }

  void DefaultHandler() {
    // Do nothing
  }

 private:
  Timer timer_;
  Milliseconds period_;
  Clock::time_point last_tick_;
  HandlerType handler_;
};

}  // namespace segment_details

}  // namespace trading

#endif