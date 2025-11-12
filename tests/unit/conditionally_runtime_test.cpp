#include <swiss/conditionally_runtime.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(conditionally_runtime, size) {
  static_assert(sizeof(swiss::conditionally_runtime<std::size_t, false, {}>) ==
                sizeof(std::monostate));
  static_assert(sizeof(swiss::conditionally_runtime<std::size_t, true, {}>) ==
                sizeof(std::size_t));
  static_assert(alignof(swiss::conditionally_runtime<std::size_t, false, {}>) ==
                sizeof(std::monostate));
  static_assert(alignof(swiss::conditionally_runtime<std::size_t, true, {}>) ==
                sizeof(std::size_t));
}

TEST(conditionally_runtime, constructor) {
  swiss::conditionally_runtime<std::size_t, false, {}>();
  swiss::conditionally_runtime<std::size_t, true>({});
}

TEST(conditionally_runtime, dereference) {
  swiss::conditionally_runtime<int, false, 1> ct_value;
  static_assert(std::is_same_v<decltype(*ct_value), int const &>);
  EXPECT_EQ(*ct_value, 1);

  swiss::conditionally_runtime<int, true> rt_value(1);
  static_assert(std::is_same_v<decltype(*rt_value), int &>);
  EXPECT_EQ(*rt_value, 1);

  swiss::conditionally_runtime<int, true> const const_rt_value(1);
  static_assert(std::is_same_v<decltype(*const_rt_value), int const &>);
  EXPECT_EQ(*const_rt_value, 1);
}

TEST(conditionally_runtime, member_access) {
  swiss::conditionally_runtime<std::pair<int, int>, false,
                               std::pair<int, int>{0, 0}> const ct_value;
  EXPECT_EQ(ct_value->first, 0);

  swiss::conditionally_runtime<std::pair<int, int>, true,
                               std::pair<int, int>{0, 0}> const
      rt_value(std::pair<int, int>{1, 1});
  EXPECT_EQ(rt_value->second, 1);
}

TEST(conditionally_runtime, value_assignment) {
  swiss::conditionally_runtime<int, true> value(1);
  EXPECT_EQ(*value, 1);

  *value = 2;
  EXPECT_EQ(*value, 2);
}

TEST(conditionally_runtime, copy_assignment) {
  swiss::conditionally_runtime<int, true> value(1);
  EXPECT_EQ(*value, 1);

  swiss::conditionally_runtime<int, true> const another_value(2);
  value = another_value;
  EXPECT_EQ(*value, 2);
}

}  // namespace tests::unit
