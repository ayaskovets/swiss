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

}  // namespace swiss
