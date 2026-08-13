#pragma once

#include <tuple>
#include <utility>

namespace swiss {

/**
 * @brief Get lvalue reference to the n-th type in template parameter pack
 */
template <std::size_t Index, typename... Args>
[[nodiscard]] constexpr auto variadic_nth(Args &&... args) -> decltype(auto) {
  return std::get<Index>(std::forward_as_tuple(std::forward<Args>(args)...));
}

/**
 * @brief Get n-th type of a non-empty variadic template pack
 */
template <std::size_t N, typename... Args>
using variadic_nth_t = std::tuple_element_t<N, std::tuple<Args...>>;

/**
 * @brief Get first type of a non-empty variadic template pack
 */
template <typename... Args>
using variadic_head_t = variadic_nth_t<0, Args...>;

}  // namespace swiss
