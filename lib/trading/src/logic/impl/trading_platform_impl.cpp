#include "logic/trading_platform.hpp"

namespace trading {

/*-----------------------------------------TradingPlatformScope------------------------------------------------*/
TradingPlatformScope::TradingPlatformImpl*
    TradingPlatformScope::trading_platform_ptr_ = nullptr;

TradingPlatformScope::TradingPlatformImpl* TradingPlatformScope::Get() {
  return trading_platform_ptr_;
}

void TradingPlatformScope::Stop() {
  trading_platform_ptr_->~TradingPlatformImpl();
}

}  // namespace trading