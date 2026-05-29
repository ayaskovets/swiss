#include <swiss/trivial_pair.hpp>

#include <utility>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(trivial_pair, smoke) {
  swiss::trivial_pair<int, int> const pair(0, 1);
  EXPECT_EQ(pair.first, 0);
  EXPECT_EQ(pair.second, 1);
  EXPECT_EQ(pair, pair);
  EXPECT_NE(pair, swiss::make_trivial_pair(0, 2));

  auto const [first, second] = pair;
  static_assert(std::is_same_v<decltype(first), int const>);
  static_assert(std::is_same_v<decltype(second), int const>);
  EXPECT_EQ(first, 0);
  EXPECT_EQ(second, 1);
}

TEST(trivial_pair, swap) {
  swiss::trivial_pair<int, int> one(0, 1);
  swiss::trivial_pair<int, int> another(0, 2);

  std::swap(one, another);
  EXPECT_EQ(one.second, 2);
  EXPECT_EQ(another.second, 1);
}

}  // namespace tests::unit
