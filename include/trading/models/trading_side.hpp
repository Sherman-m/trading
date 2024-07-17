#ifndef TRADING_SIDE_HPP
#define TRADING_SIDE_HPP

#include <cstdint>

TRADING_NAMESPACE_BEGIN

namespace models {

enum class TradingSide : std::int32_t { kBuy = 1, kSale = -1 };

}  // namespace models

TRADING_NAMESPACE_END

#endif