#pragma once

#include <concepts>

namespace swiss {

/**
 * @brief Is power of 2 greater than some value
 */
template <std::integral T>
[[nodiscard]] constexpr auto is_power_of_two_gt(T const value,
                                                T const greater_than) noexcept
    -> bool {
  return value > greater_than && ((value & (value - static_cast<T>(1))) == 0);
}

/**
 * @brief Is power of 2
 */
template <std::integral T>
[[nodiscard]] constexpr auto is_power_of_two(T const value) noexcept -> bool {
  return is_power_of_two_gt(value, static_cast<T>(0));
}

/**
 * @brief Divisible by 2
 */
template <std::integral T>
[[nodiscard]] constexpr auto is_even(T const value) noexcept -> bool {
  return !(value & 1);
}

}  // namespace swiss
