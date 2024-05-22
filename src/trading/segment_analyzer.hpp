#ifndef SEGMENT_ANALYZER_HPP
#define SEGMENT_ANALYZER_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <utility>

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
  explicit SegmentAnalyzer(net::io_context& ioc, Milliseconds period);

  SegmentAnalyzer(const SegmentAnalyzer&) = delete;
  SegmentAnalyzer& operator=(const SegmentAnalyzer&) = delete;

  SegmentAnalyzer(SegmentAnalyzer&&) = default;
  SegmentAnalyzer& operator=(SegmentAnalyzer&&) = default;

  void HandlerStart(HandlerType handler = DefaultHandler());

  void HandlerStop();

 private:
  void ScheduleTick();

  void OnTick(sys::error_code ec);

  void DefaultHandler();

 private:
  Timer timer_;
  Milliseconds period_;
  Clock::time_point last_tick_;
  HandlerType handler_;
};

}  // namespace segment_details

}  // namespace trading

#include "impl/segment_analyzer_impl.hpp"

#endif