#ifndef TRADING_SIDE_HPP
#define TRADING_SIDE_HPP

#include <cstdint>

namespace trading {

enum class TradingSide : std::int32_t { kBuy = 1, kSale = -1 };

}  // namespace trading

#endif