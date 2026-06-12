#include <swiss/functional.hpp>

#include <type_traits>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(functional, invoke_tail_func) {
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

TEST(functional, invoke_tail_class) {
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

TEST(functional, assign_invoke_result_if_not_void) {
  auto const void_result_t = [](int) {};

  auto const int_result_t = [](int) { return 2; };

  int ret = 0;

  swiss::assign_invoke_result_if_not_void(ret, void_result_t, int{});
  EXPECT_EQ(ret, 0);

  swiss::assign_invoke_result_if_not_void(ret, int_result_t, int{});
  EXPECT_EQ(ret, 2);
}

TEST(functional, noexcept_function) {
  swiss::noexcept_function<int, float> function(
      [](int arg) noexcept -> float { return static_cast<float>(arg); });

  function = nullptr;
  EXPECT_FALSE(function);

  function = [](int) noexcept -> float { return 0.F; };
  EXPECT_TRUE(function);

  static_assert(std::is_nothrow_invocable_v<decltype(function), int>);

  EXPECT_EQ(function(42), 0.F);
}

}  // namespace tests::unit
