#pragma once

namespace swiss {

/**
 * @brief Overload set for std::visit
 */
template <typename... Ts>
class overloaded : public Ts... {
 public:
  using Ts::operator()...;
};

}  // namespace swiss
