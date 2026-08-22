#pragma once

#include <concepts>
#include <type_traits>

namespace swiss {

/**
 * @brief Strong type alias
 * @note Uses pointer-like syntax for access to the implementation
 * @note Tag parameter is required to have different strong typedef instances
 * with a same underlying type but different tag
 */
template <typename Tag, typename T>
class strong_typedef {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T const &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  template <typename... Args>
    requires(std::constructible_from<T, Args...>)
  explicit constexpr strong_typedef(Args &&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>)
      : underlying_(std::forward<Args>(args)...) {}

 public:
  constexpr auto operator<=>(strong_typedef const &) const noexcept
      -> auto = default;

 public:
  constexpr auto operator*() noexcept -> reference { return underlying_; }
  constexpr auto operator*() const noexcept -> const_reference {
    return underlying_;
  }

  constexpr auto operator->() noexcept -> pointer { return &underlying_; }
  constexpr auto operator->() const noexcept -> const_pointer {
    return &underlying_;
  }

 private:
  T underlying_;
};

}  // namespace swiss
