#include <trading/core/trading_platform.hpp>

TRADING_NAMESPACE_BEGIN

namespace core {

/*-----------------------------------------TradingPlatformScope------------------------------------------------*/
TradingPlatformScope::TradingPlatformImpl*
    TradingPlatformScope::trading_platform_ptr_ = nullptr;

TradingPlatformScope::TradingPlatformImpl* TradingPlatformScope::Get() {
  return trading_platform_ptr_;
}

void TradingPlatformScope::Stop() {
  trading_platform_ptr_->~TradingPlatformImpl();
}

}  // namespace core

TRADING_NAMESPACE_END