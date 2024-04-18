#include <gtest/gtest.h>

#include <string>
#include <type_traits>
#include <tuple>
#include "../../src/util/custom_type_traits.hpp"

using namespace util::custom_type_traits;

class TupleTypesCat : public testing::Test {};

TEST_F(TupleTypesCat, GoodExpectedTwoTupleTypesCat) {
  using TupleTypes1 = std::tuple<int, double>;
  using TupleTypes2 = std::tuple<float, std::string>;
  using TupleCatResult = tuple_types_cat_t<TupleTypes1, TupleTypes2>;
  using Expected = std::tuple<int, double, float, std::string>;

  constexpr bool result = std::is_same_v<TupleCatResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleTypesCat, BadExpectedTwoTupleTypesCat) {
  using TupleTypes1 = std::tuple<int, double>;
  using TupleTypes2 = std::tuple<float, std::string>;
  using TupleCatResult = tuple_types_cat_t<TupleTypes1, TupleTypes2>;
  using Expected = std::tuple<int, double, std::string, float>;

  constexpr bool result = std::is_same_v<TupleCatResult, Expected>;
  ASSERT_FALSE(result);
}

class TupleTypes : public testing::Test {
 protected:
  struct Type1 {};
  struct Type2 {};
  struct Type3 {};
  struct Type4 {};

  using TupleTypes3Size = std::tuple<Type1, Type2, Type3>;
  using TupleTypes4Size = std::tuple<Type1, Type2, Type3, Type4>;

  template <typename Tuple>
  struct TypesChecker {
    TypesChecker() = delete;
  };
};

/*--------------------------------------using_with_object_with_one_template_arguments-----------------------------*/
class TupleObj1ArgFromTypes : public TupleTypes {
 protected:
  template <typename Arg>
  struct Obj1Arg {};
};

TEST_F(TupleObj1ArgFromTypes, GoodExpectedTupleType3Size) {
  using TupleTypeResult = tuple_obj1arg_from_types_t<Obj1Arg, TupleTypes3Size>;
  using Expected = std::tuple<Obj1Arg<Type1>, Obj1Arg<Type2>, Obj1Arg<Type3>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleObj1ArgFromTypes, BadExpectedTupleType3Size) {
  using TupleTypeResult = tuple_obj1arg_from_types_t<Obj1Arg, TupleTypes3Size>;
  using Expected = std::tuple<Obj1Arg<Type2>, Obj1Arg<Type1>, Obj1Arg<Type3>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_FALSE(result);
}

TEST_F(TupleObj1ArgFromTypes, GoodExpectedTupleType4Size) {
  using TupleTypeResult = tuple_obj1arg_from_types_t<Obj1Arg, TupleTypes4Size>;
  using Expected = std::tuple<Obj1Arg<Type1>, Obj1Arg<Type2>, Obj1Arg<Type3>,
                              Obj1Arg<Type4>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleObj1ArgFromTypes, BadExpectedTupleType4Size) {
  using TupleTypeResult = tuple_obj1arg_from_types_t<Obj1Arg, TupleTypes4Size>;
  using Expected = std::tuple<Obj1Arg<Type2>, Obj1Arg<Type1>, Obj1Arg<Type4>,
                              Obj1Arg<Type3>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_FALSE(result);
}

/*--------------------------------------using_with_object_with_two_template_arguments-----------------------------*/
class TupleObj2ArgsFromTypes : public TupleTypes {
 protected:
  template <typename Arg1, typename Arg2>
  struct Obj2Args {};
};

TEST_F(TupleObj2ArgsFromTypes, GoodExpectedTupleType3Size) {
  using TupleTypeResult =
      tuple_obj2args_from_types_t<Obj2Args, TupleTypes3Size>;
  using Expected = std::tuple<Obj2Args<Type1, Type2>, Obj2Args<Type1, Type3>,
                              Obj2Args<Type2, Type1>, Obj2Args<Type3, Type1>,
                              Obj2Args<Type2, Type3>, Obj2Args<Type3, Type2>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleObj2ArgsFromTypes, BadExpectedTupleType3Size) {
  using TupleTypeResult =
      tuple_obj2args_from_types_t<Obj2Args, TupleTypes3Size>;
  using Expected = std::tuple<Obj2Args<Type2, Type1>, Obj2Args<Type3, Type1>,
                              Obj2Args<Type1, Type2>, Obj2Args<Type1, Type3>,
                              Obj2Args<Type2, Type3>, Obj2Args<Type3, Type2>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_FALSE(result);
}

TEST_F(TupleObj2ArgsFromTypes, GoodExpectedTupleType4Size) {
  using TupleTypeResult =
      tuple_obj2args_from_types_t<Obj2Args, TupleTypes4Size>;
  using Expected = std::tuple<
      Obj2Args<Type1, Type2>, Obj2Args<Type1, Type3>, Obj2Args<Type1, Type4>,
      Obj2Args<Type2, Type1>, Obj2Args<Type3, Type1>, Obj2Args<Type4, Type1>,
      Obj2Args<Type2, Type3>, Obj2Args<Type2, Type4>, Obj2Args<Type3, Type2>,
      Obj2Args<Type4, Type2>, Obj2Args<Type3, Type4>, Obj2Args<Type4, Type3>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleObj2ArgsFromTypes, BadExpectedTupleType4Size) {
  using TupleTypeResult =
      tuple_obj2args_from_types_t<Obj2Args, TupleTypes4Size>;
  using Expected = std::tuple<
      Obj2Args<Type1, Type2>, Obj2Args<Type1, Type3>, Obj2Args<Type1, Type4>,
      Obj2Args<Type2, Type1>, Obj2Args<Type2, Type3>, Obj2Args<Type2, Type4>,
      Obj2Args<Type3, Type1>, Obj2Args<Type3, Type2>, Obj2Args<Type3, Type4>,
      Obj2Args<Type4, Type1>, Obj2Args<Type4, Type2>, Obj2Args<Type4, Type3>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_FALSE(result);
}

/*-------------using_with_object_with_three_template_arguments_where_one_of_them_is_template_template_parameter---*/
class TupleObjWithTTParams : public TupleObj2ArgsFromTypes {
 protected:
  template <typename Arg1, typename Arg2,
            template <typename, typename> typename TTParams>
  struct ObjWithTTParams {};

  template <typename Arg1, typename Arg2>
  using ObjType = ObjWithTTParams<Arg1, Arg2, Obj2Args>;
};

TEST_F(TupleObjWithTTParams, GoodExpectedTupleType3Size) {
  using TupleTypeResult = tuple_obj2args_from_types_t<ObjType, TupleTypes3Size>;
  using Expected = std::tuple<ObjWithTTParams<Type1, Type2, Obj2Args>,
                              ObjWithTTParams<Type1, Type3, Obj2Args>,
                              ObjWithTTParams<Type2, Type1, Obj2Args>,
                              ObjWithTTParams<Type3, Type1, Obj2Args>,
                              ObjWithTTParams<Type2, Type3, Obj2Args>,
                              ObjWithTTParams<Type3, Type2, Obj2Args>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleObjWithTTParams, BadExpectedTupleType3Size) {
  using TupleTypeResult = tuple_obj2args_from_types_t<ObjType, TupleTypes4Size>;
  using Expected = std::tuple<ObjWithTTParams<Type1, Type2, Obj2Args>,
                              ObjWithTTParams<Type1, Type3, Obj2Args>,
                              ObjWithTTParams<Type2, Type1, Obj2Args>,
                              ObjWithTTParams<Type2, Type3, Obj2Args>,
                              ObjWithTTParams<Type3, Type1, Obj2Args>,
                              ObjWithTTParams<Type3, Type2, Obj2Args>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_FALSE(result);
}

TEST_F(TupleObjWithTTParams, GoodExpectedTupleType4Size) {
  using TupleTypeResult = tuple_obj2args_from_types_t<ObjType, TupleTypes4Size>;
  using Expected = std::tuple<ObjWithTTParams<Type1, Type2, Obj2Args>,
                              ObjWithTTParams<Type1, Type3, Obj2Args>,
                              ObjWithTTParams<Type1, Type4, Obj2Args>,
                              ObjWithTTParams<Type2, Type1, Obj2Args>,
                              ObjWithTTParams<Type3, Type1, Obj2Args>,
                              ObjWithTTParams<Type4, Type1, Obj2Args>,
                              ObjWithTTParams<Type2, Type3, Obj2Args>,
                              ObjWithTTParams<Type2, Type4, Obj2Args>,
                              ObjWithTTParams<Type3, Type2, Obj2Args>,
                              ObjWithTTParams<Type4, Type2, Obj2Args>,
                              ObjWithTTParams<Type3, Type4, Obj2Args>,
                              ObjWithTTParams<Type4, Type3, Obj2Args>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_TRUE(result);
}

TEST_F(TupleObjWithTTParams, BadExpectedTupleType4Size) {
  using TupleTypeResult = tuple_obj2args_from_types_t<ObjType, TupleTypes4Size>;
  using Expected = std::tuple<ObjWithTTParams<Type1, Type2, Obj2Args>,
                              ObjWithTTParams<Type1, Type3, Obj2Args>,
                              ObjWithTTParams<Type1, Type4, Obj2Args>,
                              ObjWithTTParams<Type2, Type1, Obj2Args>,
                              ObjWithTTParams<Type2, Type3, Obj2Args>,
                              ObjWithTTParams<Type2, Type4, Obj2Args>,
                              ObjWithTTParams<Type3, Type1, Obj2Args>,
                              ObjWithTTParams<Type3, Type2, Obj2Args>,
                              ObjWithTTParams<Type3, Type4, Obj2Args>,
                              ObjWithTTParams<Type4, Type1, Obj2Args>,
                              ObjWithTTParams<Type4, Type2, Obj2Args>,
                              ObjWithTTParams<Type4, Type3, Obj2Args>>;

  constexpr bool result = std::is_same_v<TupleTypeResult, Expected>;
  ASSERT_FALSE(result);
}