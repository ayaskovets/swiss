#pragma once

namespace swiss {

template <typename... Ts>
class overloaded : public Ts... {
 public:
  using Ts::operator()...;
};

}  // namespace swiss
