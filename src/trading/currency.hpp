#ifndef CURRENCY_HPP
#define CURRENCY_HPP

#include "../util/tagged_numeric.hpp"
#include "../util/custom_type_traits.hpp"
#include <cinttypes>
#include <tuple>

namespace trading {
namespace currencies {

namespace detail {

using CurrencyNumericType = int64_t;

template <typename Tag>
using Currency = util::TaggedNumeric<CurrencyNumericType, Tag>;

}  // namespace detail

/*-------------------------------------------TypesCurrencies---------------------------------------------------*/
struct UsdTag {};
using Usd = detail::Currency<UsdTag>;

struct RublesTag {};
using Rubles = detail::Currency<RublesTag>;

/*-------------------------------------------------------------------------------------------------------------*/

using TypesCurrencies = std::tuple<Usd, Rubles>;

template <template <typename> typename Object>
using TupleObj1ArgFromCurrencyTypesT =
    util::custom_type_traits::tuple_obj1arg_from_types_t<Object,
                                                         TypesCurrencies>;

template <template <typename, typename> typename Object>
using TupleObj2ArgsFromCurrencyTypesT =
    util::custom_type_traits::tuple_obj2args_from_types_t<Object,
                                                          TypesCurrencies>;

}  // namespace currencies

}  // namespace trading
#endif