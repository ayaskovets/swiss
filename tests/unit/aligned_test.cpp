#include <swiss/aligned.hpp>

#include <gtest/gtest.h>

namespace tests::unit {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

TEST(aligned, size) {
  static_assert(sizeof(swiss::aligned<std::size_t, alignof(std::size_t)>) ==
                sizeof(std::size_t));
  static_assert(sizeof(swiss::aligned<std::size_t, 16>) == 16);
  static_assert(alignof(swiss::aligned<std::size_t, alignof(std::size_t)>) ==
                alignof(std::size_t));
  static_assert(alignof(swiss::aligned<std::size_t, 16>) == 16);
}

TEST(aligned, constructor) {
  swiss::aligned<std::size_t, 16>();
  swiss::aligned<std::size_t, 16>(2U);
}

TEST(aligned, comparison) {
  EXPECT_EQ((swiss::aligned<int, 32>(1)), (swiss::aligned<int, 32>(1)));
  EXPECT_NE((swiss::aligned<int, 32>(1)), (swiss::aligned<int, 32>(2)));
}

TEST(aligned, dereference) {
  swiss::aligned<int, 16> value(1);
  static_assert(std::is_same_v<decltype(*value), int &>);
  EXPECT_EQ(*value, 1);

  swiss::aligned<int, 16> const const_value(1);
  static_assert(std::is_same_v<decltype(*const_value), int const &>);
  EXPECT_EQ(*const_value, 1);
}

TEST(aligned, member_access) {
  swiss::aligned<std::pair<int, int>, 32> const value;
  EXPECT_EQ(value->first, 0);
}

TEST(aligned, value_assignment) {
  swiss::aligned<int, 16> value(1);
  EXPECT_EQ(*value, 1);

  *value = 2;
  EXPECT_EQ(*value, 2);
}

TEST(aligned, copy_assignment) {
  swiss::aligned<int, 16> value(1);
  EXPECT_EQ(*value, 1);

  swiss::aligned<int, 16> const another_value(2);
  value = another_value;
  EXPECT_EQ(*value, 2);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}  // namespace tests::unit