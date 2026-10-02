#include <swiss/frozen_flat_set.hpp>

#include <iterator>
#include <ranges>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(frozen_flat_set, concepts) {
  using set_type = swiss::frozen_flat_set<int, 3>;

  static_assert(std::ranges::contiguous_range<set_type>);
  static_assert(std::contiguous_iterator<set_type::iterator>);
}

TEST(frozen_flat_set, access) {
  swiss::frozen_flat_set<int, 3> set({3, 2, 1});

  EXPECT_EQ(set.size(), 3);
  EXPECT_FALSE(set.empty());

  EXPECT_EQ(set[0], 1);
  EXPECT_EQ(set[1], 2);
  EXPECT_EQ(set[2], 3);
}

TEST(frozen_flat_set, compare) {
  swiss::frozen_flat_set<int, 3, std::greater<>> set({3, 2, 1});

  EXPECT_EQ(set[0], 3);
  EXPECT_EQ(set[1], 2);
  EXPECT_EQ(set[2], 1);
}

}  // namespace tests::unit
