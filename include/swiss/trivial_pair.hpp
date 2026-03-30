#pragma once

#include <type_traits>

namespace swiss {

/**
 * @brief Poor man's std::pair with trivially copyable types
 * @note The main motivation for this type is to have an std::atomic of
 * std::pair-like aggregate which requires the underlying type to be
 * memcpy-copyable
 */
template <typename T, typename U>
  requires(std::is_trivial_v<T> && std::is_trivial_v<U>)
struct trivial_pair {
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  T first;
  U second;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  constexpr auto operator<=>(trivial_pair const &) const noexcept = default;
};

template <typename T, typename U>
constexpr auto make_trivial_pair(T && first, U && second)
    -> trivial_pair<T, U> {
  return trivial_pair(std::forward<T>(first), std::forward<U>(second));
}

}  // namespace swiss
