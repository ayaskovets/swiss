#include <swiss/functional.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(functional, invoke_tail) {
  // NOLINTBEGIN(readability-identifier-length)
  auto const invocable = [](int const & i, char const &,
                            float const &) noexcept { return i; };

  std::string const s{};
  int const i   = 1;
  char const c  = 2;
  float const f = 3;

  EXPECT_EQ(swiss::invoke_tail(invocable, s, i, c, f), i);
  static_assert(noexcept(swiss::invoke_tail(invocable, s, i, c, f)));

  // NOLINTEND(readability-identifier-length)
}

TEST(functional, capture_this) {
  struct Object final {
    [[nodiscard]] constexpr auto const_method(int addition) const noexcept
        -> int {
      return base + addition;
    }

    [[nodiscard]] constexpr auto mutable_method(int addition) noexcept -> int {
      return ++base + addition;
    }

    [[nodiscard]] constexpr auto throw_method(int addition) const -> int {
      return base + addition;
    }

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    int base;
  } object{.base = 1};

  EXPECT_EQ(
      swiss::invoke_tail(object, &Object::mutable_method, std::string{}, 3), 5);
  static_assert(noexcept(
      swiss::invoke_tail(object, &Object::mutable_method, std::string{}, 3)));

  EXPECT_EQ(swiss::invoke_tail(object, &Object::const_method, std::string{}, 3),
            5);
  static_assert(noexcept(
      swiss::invoke_tail(object, &Object::const_method, std::string{}, 3)));

  EXPECT_EQ(swiss::invoke_tail(object, &Object::throw_method, std::string{}, 3),
            5);
  static_assert(!noexcept(
      swiss::invoke_tail(object, &Object::throw_method, std::string{}, 3)));
}

}  // namespace tests::unit
