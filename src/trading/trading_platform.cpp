#include "trading_platform.hpp"

namespace trading {

/*-------------------------------------------------TradingPlatformScope----------------------------------------------------------------*/
trading_platform_details::TradingPlatformImpl*
    TradingPlatformScope::trading_platform_ptr_ = nullptr;

trading_platform_details::TradingPlatformImpl* TradingPlatformScope::Get() {
  return trading_platform_ptr_;
}

void TradingPlatformScope::Stop() {
  trading_platform_ptr_->~TradingPlatformImpl();
}

}  // namespace trading