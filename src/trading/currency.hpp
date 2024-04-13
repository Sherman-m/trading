#ifndef CURRENCY_HPP
#define CURRENCY_HPP

#include "../util/tagged_numeric.hpp"
#include <cinttypes>

namespace trading {
namespace currencies {

namespace detail {

using CurrencyNumericType = int64_t;

template <typename Tag>
using Currency = util::TaggedNumeric<CurrencyNumericType, Tag>;

}  // namespace detail

struct UsdTag {};
using Usd = detail::Currency<UsdTag>;

struct RublesTag {};
using Rubles = detail::Currency<RublesTag>;

}  // namespace currencies

}  // namespace trading
#endif