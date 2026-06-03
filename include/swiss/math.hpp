#pragma once

#include <concepts>
#include <limits>
#include <utility>

namespace swiss {

/**
 * @brief Safely clamp a value to the [min, max] interval for given target type
 */
template <std::integral To>
constexpr auto bound(std::integral auto value) noexcept -> To {
  constexpr To kMin = std::numeric_limits<To>::min();
  constexpr To kMax = std::numeric_limits<To>::max();

  if (std::cmp_less(value, kMin)) {
    return kMin;
  }

  if (std::cmp_greater(value, kMax)) {
    return kMax;
  }

  return static_cast<To>(value);
}

}  // namespace swiss
