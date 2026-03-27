#pragma once

#include <tuple>
#include <utility>

namespace swiss {

template <std::size_t Index, typename... Args>
constexpr auto variadic_nth(Args &&... args) -> decltype(auto) {
  return std::get<Index>(std::forward_as_tuple(std::forward<Args>(args)...));
}

}  // namespace swiss
