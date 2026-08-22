#pragma once

#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Empty base optimisation for scenarios when a value is possibly known
 * at compile time
 * @note Uses pointer-like syntax for access to the stored value
 * @note Add [[no_unique_address]] to a compile-time version of an object of
 * this class to optimize away storage at runtime
 */
template <typename T, bool IsRuntime, T CompileTimeValue = T()>
class ebo;

template <typename T, T CompileTimeValue>
class ebo<T, true, CompileTimeValue> {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T const &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  constexpr ebo() = delete;
  explicit constexpr ebo(T value) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : value_(std::move(value)) {}

 public:
  constexpr auto operator*() noexcept -> reference { return value_; }
  constexpr auto operator*() const noexcept -> const_reference {
    return value_;
  }

  constexpr auto operator->() noexcept -> pointer { return &value_; };
  constexpr auto operator->() const noexcept -> const_pointer {
    return &value_;
  };

 private:
  T value_;
};

template <typename T, T CompileTimeValue>
class ebo<T, false, CompileTimeValue> {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T const &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  constexpr ebo() noexcept = default;

 public:
  constexpr auto operator*() const noexcept -> const_reference {
    return value_;
  }
  constexpr auto operator->() const noexcept -> const_pointer {
    return &value_;
  };

 private:
  static constexpr T const value_ = CompileTimeValue;
};

}  // namespace swiss
