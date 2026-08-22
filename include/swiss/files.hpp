#pragma once

#include <cstdio>
#include <tuple>

namespace swiss {

class fcloser final {
 public:
  constexpr void operator()(FILE * file) {
    if (file != nullptr) {
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
      std::ignore = std::fclose(file);
    }
  }
};

}  // namespace swiss
