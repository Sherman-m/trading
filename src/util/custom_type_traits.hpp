#ifndef CUSTOM_TYPE_TRAITS
#define CUSTOM_TYPE_TRAITS

#include <tuple>

namespace util {

namespace custom_type_traits {

/*-----------------------------------tuple_types_concatenation----------------------------------*/
template <class, class>
struct tuple_types_cat;

template <class... TupleArgs1, class... TupleArgs2>
struct tuple_types_cat<std::tuple<TupleArgs1...>, std::tuple<TupleArgs2...>> {
  using type = std::tuple<TupleArgs1..., TupleArgs2...>;
};

template <class Tuple1, class Tuple2>
using tuple_types_cat_t = typename tuple_types_cat<Tuple1, Tuple2>::type;

/*-----------------------------------tuple_objects_from_types-----------------------------------*/
/*------------------------------------objects_with_one_arg--------------------------------------*/
template <template <typename> typename Object, typename TupleTypes>
struct tuple_obj1arg_from_types;

template <template <typename> typename Object, typename... Types>
struct tuple_obj1arg_from_types<Object, std::tuple<Types...>> {
  using type = std::tuple<Object<Types>...>;
};

template <template <typename> typename Object, typename TupleTypes>
using tuple_obj1arg_from_types_t =
    typename tuple_obj1arg_from_types<Object, TupleTypes>::type;

/*-----------------------------------objects_with_two_args--------------------------------------*/
template <template <typename, typename> typename Object, typename TupleTypes>
struct tuple_obj2args_from_types;

template <template <typename, typename> typename Object, typename T,
          typename... Types>
struct tuple_obj2args_from_types<Object, std::tuple<T, Types...>> {
  using type =
      tuple_types_cat<std::tuple<Object<T, Types>..., Object<Types, T>...>,
                      typename tuple_obj2args_from_types<
                          Object, std::tuple<Types...>>::type>::type;
};

template <template <typename, typename> typename Object, typename T>
struct tuple_obj2args_from_types<Object, std::tuple<T>> {
  using type = std::tuple<>;
};

template <template <typename, typename> typename Object, typename TupleTypes>
using tuple_obj2args_from_types_t =
    typename tuple_obj2args_from_types<Object, TupleTypes>::type;

}  // namespace custom_type_traits

}  // namespace util
#endif