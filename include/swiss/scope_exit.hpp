#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Declarative way to do an noexcept operation when exiting a scope
 */
template <std::invocable T>
class scope_exit final {
 private:
  static_assert(noexcept(std::declval<T>()()));

 public:
  constexpr explicit scope_exit(T finally) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : finally_(std::move(finally)) {}
  constexpr ~scope_exit() noexcept { finally_(); }

 public:
  constexpr explicit scope_exit() noexcept              = delete;
  constexpr scope_exit(scope_exit const &) noexcept     = delete;
  constexpr scope_exit(scope_exit &&) noexcept          = delete;
  constexpr auto operator=(scope_exit const &) noexcept = delete;
  constexpr auto operator=(scope_exit &&) noexcept      = delete;

 private:
  T finally_;
};

}  // namespace swiss
