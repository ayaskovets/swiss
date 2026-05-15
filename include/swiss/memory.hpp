#pragma once

#include <cstddef>

namespace swiss {

constexpr auto align_up(std::size_t bytes, std::size_t alignment) noexcept
    -> std::size_t {
  return (bytes + alignment - 1U) & ~(alignment - 1U);
}

}  // namespace swiss
