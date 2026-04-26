#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace swiss {

template <typename T, std::size_t Alignment>
class alignas(Alignment) aligned {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  constexpr aligned() noexcept(std::is_nothrow_default_constructible_v<T>)
    requires(std::is_default_constructible_v<T>)
      : value_() {}

  explicit constexpr aligned(T value) noexcept(
      std::is_nothrow_move_constructible_v<T>)
    requires(std::constructible_from<T, T &&>)
      : value_(std::move(value)) {}

 public:
  constexpr auto operator*() noexcept -> T & { return value_; }
  constexpr auto operator*() const noexcept -> T const & { return value_; }
  constexpr auto operator->() noexcept -> T * { return &value_; };
  constexpr auto operator->() const noexcept -> T const * { return &value_; };

 private:
  T value_;
};

}  // namespace swiss
