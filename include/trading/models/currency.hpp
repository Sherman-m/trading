#ifndef CURRENCY_HPP
#define CURRENCY_HPP

#include <cstdint>
#include <trading/util/custom_type_traits.hpp>
#include <trading/util/tagged_numeric.hpp>
#include <tuple>

TRADING_NAMESPACE_BEGIN

namespace models {

namespace currencies {

/*-----------------------------------------CurrencyDetails-----------------------------------------------------*/
namespace details {

using CurrencyNumericType = std::int64_t;

template <typename Tag>
using Currency = util::TaggedNumeric<CurrencyNumericType, Tag>;

}  // namespace details

/*-----------------------------------------TupleCurrencyTypes-----------------------------------------------------*/
struct UsdTag {};
using Usd = details::Currency<UsdTag>;

struct RublesTag {};
using Rubles = details::Currency<RublesTag>;

using TupleCurrencyTypes = std::tuple<Usd, Rubles>;

/*-----------------------------------------CurrenciesTraits----------------------------------------------------*/
template <template <typename> typename Object>
using TupleObj1ArgFromCurrencyTypesT =
    util::custom_type_traits::tuple_obj1arg_from_types_t<Object,
                                                         TupleCurrencyTypes>;

template <template <typename, typename> typename Object>
using TupleObj2ArgsFromCurrencyTypesT =
    util::custom_type_traits::tuple_obj2args_from_types_t<Object,
                                                          TupleCurrencyTypes>;

}  // namespace currencies

}  // namespace models

TRADING_NAMESPACE_END

#endif