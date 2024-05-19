#include <iostream>
#include "trading/trading.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <chrono>

using namespace std::chrono_literals;
using namespace trading;

namespace net = boost::asio;
namespace sys = boost::system;

using OrderType =
    trading::Order<trading::currencies::Usd, trading::currencies::Rubles>;

int main() {
  boost::asio::io_context ioc;
  trading::SegmentConfig<storages::DefaultStorage> config(ioc, 50ms);

  TradingPlatformScope::Run(config);

  net::signal_set signals(ioc, SIGINT, SIGTERM, SIGHUP);
  signals.async_wait([&ioc](const sys::error_code& ec,
                            [[maybe_unused]] int32_t signal_number) {
    if (ec) {
      throw std::runtime_error("Error signal receiving");
    };
    ioc.stop();
  });

  MarketMember mm1{1};
  MarketMember mm2{2};

  mm1.BuyCurrency<currencies::Usd>(30ul, currencies::Rubles(50));

  mm2.SaleCurrency<currencies::Usd>(40ul, currencies::Rubles(40));

  ioc.run();

  TradingPlatformScope::Stop();
  return EXIT_SUCCESS;
}