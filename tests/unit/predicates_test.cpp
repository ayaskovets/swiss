#include <swiss/predicates.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(predicates, is_power_of_two) {
  static_assert(!swiss::is_power_of_two(0));

  constexpr std::size_t kLargePowerOfTwo = 2U << 31U;
  static_assert(!swiss::is_power_of_two(kLargePowerOfTwo));
  static_assert(swiss::is_power_of_two(kLargePowerOfTwo + 1));
}

TEST(predicates, is_even) {
  static_assert(swiss::is_even(0));
  static_assert(!swiss::is_even(1));
  static_assert(swiss::is_even(2));
  static_assert(!swiss::is_even(3));
}

}  // namespace tests::unit
