#pragma once

#include <concepts>
#include <exception>
#include <type_traits>
#include <utility>

namespace swiss {

template <std::invocable T>
class scope_fail final {
 private:
  static_assert(std::is_nothrow_invocable_v<T>);

 public:
  explicit constexpr scope_fail(T finally) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : finally_(std::move(finally)),
        uncaught_initial_(std::uncaught_exceptions()) {}
  constexpr ~scope_fail() noexcept {
    if (std::uncaught_exceptions() > uncaught_initial_) {
      finally_();
    }
  }

 public:
  explicit constexpr scope_fail() noexcept              = delete;
  constexpr scope_fail(scope_fail const &) noexcept     = delete;
  constexpr scope_fail(scope_fail &&) noexcept          = delete;
  constexpr auto operator=(scope_fail const &) noexcept = delete;
  constexpr auto operator=(scope_fail &&) noexcept      = delete;

 private:
  T finally_;
  int const uncaught_initial_;
};

}  // namespace swiss
