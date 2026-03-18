#pragma once

#include <type_traits>

namespace swiss {

/**
 * @brief Strong type alias
 *
 * Uses pointer-like syntax for access to the implementation
 */
template <typename Tag, typename T>
class strong_typedef final {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = T &;
  using pointer = T *;
  using const_pointer = T const *;

 public:
  template <typename... Args>
  constexpr explicit strong_typedef(Args &&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>)
      : underlying_(std::forward<Args>(args)...) {}

 public:
  constexpr auto operator<=>(strong_typedef const &) const noexcept
      -> auto = default;

 public:
  constexpr auto operator*() noexcept -> T & { return underlying_; }
  constexpr auto operator*() const noexcept -> T const & { return underlying_; }

  constexpr auto operator->() noexcept -> T * { return &underlying_; }
  constexpr auto operator->() const noexcept -> T const * {
    return &underlying_;
  }

 private:
  T underlying_;
};

}  // namespace swiss
