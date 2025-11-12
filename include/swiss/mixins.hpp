#pragma once

namespace swiss {

/**
 * @brief Mix-in to restrict copying
 *
 * Please follow the rule of five for all derived types
 */
class non_copyable {
 public:
  constexpr non_copyable() noexcept = default;
  constexpr ~non_copyable() noexcept = default;
  constexpr non_copyable(non_copyable &&) noexcept = default;
  constexpr auto operator=(non_copyable &&) noexcept
      -> non_copyable & = default;

 public:
  constexpr non_copyable(non_copyable const &) = delete;
  constexpr auto operator=(non_copyable const &) = delete;
};

/**
 * @brief Mix-in to restrict moving
 *
 * Please follow the rule of five for all derived types
 *
 * @warning Keep in mind that all operations with a type that prohibits only its
 * moves will fallback to copy
 */
class non_movable {
 public:
  constexpr non_movable() noexcept = default;
  constexpr ~non_movable() noexcept = default;
  constexpr non_movable(non_movable const &) noexcept = default;
  constexpr auto operator=(non_movable const &) noexcept
      -> non_movable & = default;

 public:
  constexpr non_movable(non_movable &&) = delete;
  constexpr auto operator=(non_movable &&) = delete;
};

}  // namespace swiss
