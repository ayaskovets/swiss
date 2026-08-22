#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace swiss {

template <typename T, std::size_t Alignment>
  requires(Alignment >= alignof(T))
class alignas(Alignment) aligned {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T const &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  template <typename... Args>
    requires(std::constructible_from<T, Args...>)
  explicit constexpr aligned(Args &&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>)
      : value_(std::forward<Args>(args)...) {}

 public:
  constexpr auto operator<=>(aligned const & that) const noexcept = default;

 public:
  constexpr auto operator*() noexcept -> reference { return value_; }
  constexpr auto operator*() const noexcept -> const_reference {
    return value_;
  }

  constexpr auto operator->() noexcept -> pointer { return &value_; };
  constexpr auto operator->() const noexcept -> const_pointer {
    return &value_;
  }

 private:
  alignas(Alignment) T value_;
};

}  // namespace swiss
