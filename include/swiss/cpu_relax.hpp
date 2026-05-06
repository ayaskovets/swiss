#pragma once

namespace swiss {

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)

#include <intrin.h>
constexpr void cpu_relax() noexcept { _mm_pause(); }

#elif defined(__GNUC__) || defined(__clang__)

#if defined(__i386__) || defined(__x86_64__)

#include <xmmintrin.h>
constexpr void cpu_relax() noexcept { _mm_pause(); }

#elif defined(__arm__) || defined(__aarch64__)

constexpr void cpu_relax() noexcept {
  // NOLINTNEXTLINE(hicpp-no-assembler)
  __asm__ __volatile__("yield" ::: "memory");
}

#elif defined(__powerpc__)

constexpr void cpu_relax() noexcept {
  // NOLINTNEXTLINE(hicpp-no-assembler)
  __asm__ __volatile__("or 27,27,27" ::: "memory");
}

#endif

#endif

}  // namespace swiss
