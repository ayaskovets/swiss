#pragma once

#include <concepts>
#include <limits>

namespace swiss {

/**
 * @brief Type-parametrized std::dynamic_extent
 */
template <std::unsigned_integral T, T Value = std::numeric_limits<T>::max()>
constexpr T const kDynamicExtent = Value;

}  // namespace swiss
