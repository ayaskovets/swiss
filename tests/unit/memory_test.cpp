#include <swiss/memory.hpp>

#include <gtest/gtest.h>

namespace tests::unit {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

TEST(memory, align_up) {
  static_assert(swiss::align_up(1, 8) == 8);
  static_assert(swiss::align_up(2, 8) == 8);
  static_assert(swiss::align_up(3, 8) == 8);
  static_assert(swiss::align_up(4, 8) == 8);
  static_assert(swiss::align_up(5, 8) == 8);
  static_assert(swiss::align_up(6, 8) == 8);
  static_assert(swiss::align_up(7, 8) == 8);
  static_assert(swiss::align_up(8, 8) == 8);
  static_assert(swiss::align_up(9, 8) == 16);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}  // namespace tests::unit