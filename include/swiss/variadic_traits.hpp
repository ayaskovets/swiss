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

}  // namespace swiss
