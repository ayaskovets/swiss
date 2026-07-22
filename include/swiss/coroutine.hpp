#pragma once

#include <coroutine>

namespace swiss {
// NOLINTBEGIN(readability-convert-member-functions-to-static)

/**
 * @brief Naive mock implementation of a coroutine task
 */
class task final {
 public:
  struct promise_type final {
    constexpr auto get_return_object() noexcept -> task {
      return task(std::coroutine_handle<promise_type>::from_promise(*this));
    }
    constexpr auto initial_suspend() noexcept -> std::suspend_always {
      return {};
    }
    constexpr auto final_suspend() noexcept -> std::suspend_always {
      return {};
    }
    constexpr void unhandled_exception() noexcept {}
    constexpr void return_void() noexcept {}
  };

 public:
  explicit constexpr task(std::coroutine_handle<promise_type> handle)
      : handle_(handle) {}

 public:
  [[nodiscard]] constexpr auto handle() const noexcept
      -> std::coroutine_handle<promise_type> {
    return handle_;
  }

 private:
  std::coroutine_handle<promise_type> handle_;
};

// NOLINTEND(readability-convert-member-functions-to-static)
}  // namespace swiss
