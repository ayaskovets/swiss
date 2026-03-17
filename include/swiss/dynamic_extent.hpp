#pragma once

#include <concepts>

namespace swiss {

/**
 * @brief Type-parametrized std::dynamic_extent with 0 representing a statically
 * undetermined value
 */
template <std::unsigned_integral T, T Value = static_cast<T>(0)>
constexpr T const kDynamicExtent = Value;

}  // namespace swiss
