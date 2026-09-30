#pragma once

namespace swiss {

/**
 * @brief Utility class for token-based access to private methods instead of
 * sharing the whole class via friend
 */
template <typename T>
class badge final {
 private:
  friend T;
  constexpr badge() noexcept = default;
};

}  // namespace swiss
