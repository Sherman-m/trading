#ifndef CONFIG_BASE_HPP
#define CONFIG_BASE_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <chrono>
#include <cstdint>

TRADING_NAMESPACE_BEGIN

namespace configs {

namespace net = boost::asio;
namespace sys = boost::system;

template <template <typename> typename Storage,
          template <typename> typename MarketMemberStorage>
struct ConfigBase {
  template <typename StorageConfig>
  using StorageType = Storage<StorageConfig>;

  template <typename MarketMemberConfig>
  using MarketMemberStorageType = MarketMemberStorage<MarketMemberConfig>;

  using Milliseconds = std::chrono::milliseconds;

  ConfigBase() = default;

  explicit ConfigBase(net::io_context& ioc, Milliseconds analyze_period)
      : ioc_(ioc),
        analyze_period_(analyze_period) {
  }

  virtual void Prepare() {
    // Do nothing;
  }

  void Setup() {
    net::signal_set signals(ioc_, SIGINT, SIGTERM, SIGHUP);
    signals.async_wait(
        [&ioc_ = this->ioc_](const sys::error_code& ec,
                             [[maybe_unused]] std::int32_t signal_number) {
          if (ec) {
            throw std::runtime_error("Error signal receiving");
          };
          ioc_.stop();
        });

    Prepare();
  }

  net::io_context& ioc_;
  Milliseconds analyze_period_{std::chrono::milliseconds(30)};
};

}  // namespace configs

TRADING_NAMESPACE_END

#endif