#pragma once

#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Conditionally runtime-stored value
 * @note Uses pointer-like syntax for access to the stored value
 * @note Add [[no_unique_address]] to a compile-time version of an object of
 * this class to optimize away storage at runtime
 */
template <typename T, bool IsRuntime, T CompileTimeValue = T()>
class conditionally_runtime;

template <typename T, T CompileTimeValue>
class conditionally_runtime<T, true, CompileTimeValue> {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  constexpr conditionally_runtime() = delete;
  constexpr explicit conditionally_runtime(T value) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : value_(std::move(value)) {}

 public:
  constexpr auto operator*() noexcept -> T & { return value_; }
  constexpr auto operator*() const noexcept -> T const & { return value_; }
  constexpr auto operator->() noexcept -> T * { return &value_; };
  constexpr auto operator->() const noexcept -> T const * { return &value_; };

 private:
  T value_;
};

template <typename T, T CompileTimeValue>
class conditionally_runtime<T, false, CompileTimeValue> {
 public:
  constexpr conditionally_runtime() noexcept = default;

  constexpr auto operator*() const noexcept -> T const & { return value_; }
  constexpr auto operator->() const noexcept -> T const * { return &value_; };

 private:
  static inline const constinit T value_ = CompileTimeValue;
};

}  // namespace swiss
