#include <swiss/variadic_traits.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(variadic_traits, variadic_nth) {
  EXPECT_EQ(swiss::variadic_nth<0>(0, 1, 2), 0);
  EXPECT_EQ(swiss::variadic_nth<1>(0, 1, 2), 1);
  EXPECT_EQ(swiss::variadic_nth<2>(0, 1, 2), 2);

  std::unique_ptr<int> const non_copyable{};
  EXPECT_EQ(swiss::variadic_nth<0>(non_copyable), non_copyable);
}

TEST(variadic_traits, variadic_nth_t) {
  static_assert(
      std::is_same_v<swiss::variadic_nth_t<2, int, float, char>, char>);
}

TEST(variadic_traits, variadic_head_t) {
  static_assert(std::is_same_v<swiss::variadic_head_t<int, float, char>, int>);
}

}  // namespace tests::unit
