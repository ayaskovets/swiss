#include <swiss/concepts.hpp>

#include <coroutine>
#include <vector>

#include <gtest/gtest.h>

namespace tests::unit {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace {

struct global_operator_awaiter final {};
constexpr auto operator co_await(global_operator_awaiter) noexcept
    -> std::suspend_always {
  return {};
}

struct member_operator_awaiter final {
  constexpr auto operator co_await() const noexcept -> std::suspend_always {
    return {};
  }
};

}  // namespace

TEST(concepts, void_invocable) {
  static_assert(swiss::void_invocable<decltype([]() -> void {})>);
  static_assert(!swiss::void_invocable<decltype([]() -> int { return 0; })>);
}

TEST(concepts, nothrow_void_invocable) {
  static_assert(
      swiss::nothrow_void_invocable<decltype([]() noexcept -> void {})>);
  static_assert(!swiss::nothrow_void_invocable<decltype([]() noexcept -> int {
    return 0;
  })>);
}

TEST(concepts, not_same_as) {
  static_assert(swiss::not_same_as<int, float>);
  static_assert(!swiss::not_same_as<int, int>);
}

TEST(concepts, not_convertible_to) {
  static_assert(!swiss::not_convertible_to<int, float>);
  static_assert(swiss::not_convertible_to<int, int *>);
}

TEST(concepts, decay_same_as) {
  static_assert(swiss::decay_same_as<int * const volatile, int *>);
}

TEST(concepts, instantiation_of) {
  static_assert(swiss::instantiation_of<std::vector<int>, std::vector>);
}

TEST(concepts, one_of) {
  static_assert(swiss::one_of<int, float, char, int>);
  static_assert(!swiss::one_of<int, float, char, double>);
}

TEST(concepts, co_await_result_v) {
  static_assert(
      std::is_same_v<swiss::co_await_result_t<global_operator_awaiter>,
                     std::suspend_always>);
  static_assert(
      std::is_same_v<swiss::co_await_result_t<member_operator_awaiter>,
                     std::suspend_always>);
  static_assert(std::is_same_v<swiss::co_await_result_t<std::suspend_always>,
                               std::suspend_always>);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}  // namespace tests::unit