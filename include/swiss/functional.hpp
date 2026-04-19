#pragma once

#include <concepts>

namespace swiss {

/**
 * @brief Invoke a callable with all parameters in pack except the first one
 */
template <typename Head, typename... Tail>
constexpr auto invoke_tail(
    std::invocable<Tail...> auto && invocable, Head &&,
    Tail &&... tail) noexcept(std::is_nothrow_invocable_v<decltype(invocable),
                                                          Tail...>)
    -> decltype(auto) {
  return std::invoke(std::forward<decltype(invocable)>(invocable),
                     std::forward<Tail>(tail)...);
}

/**
 * @brief Invoke a class method with all parameters in pack except the first one
 */
template <typename Head, typename... Tail>
constexpr auto invoke_tail(
    auto && _this, auto const & method, Head &&,
    Tail &&... tail) noexcept(std::is_nothrow_invocable_v<decltype(method),
                                                          decltype(_this),
                                                          Tail...>)
    -> decltype(auto)
  requires(std::invocable<decltype(method), decltype(_this), Tail...>)
{
  return std::invoke(method, std::forward<decltype(_this)>(_this),
                     std::forward<Tail>(tail)...);
}

template <typename T, typename Callable, typename... Args>
constexpr void assign_invoke_result_if_not_void(
    T & ret, Callable && callable,
    Args &&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
  if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
    std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
  } else {
    ret = std::invoke(std::forward<Callable>(callable),
                      std::forward<Args>(args)...);
  }
}

}  // namespace swiss
