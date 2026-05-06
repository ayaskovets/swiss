#pragma once

#include <cstdint>
#include <format>

namespace swiss {

enum class cpu_arch : std::uint8_t {
  kx86_64,
  kx86_32,
  kARM64,
  kARM32,
  kPowerPC
};

constexpr auto get_cpu_arch() noexcept -> cpu_arch {
#if defined(__x86_64__) || defined(_M_X64)
  return cpu_arch::kx86_64;
#elif defined(__i386__) || defined(_M_IX86)
  return cpu_arch::kx86_32;
#elif defined(__aarch64__) || defined(_M_ARM64)
  return cpu_arch::kARM64;
#elif defined(__arm__) || defined(_M_ARM)
  return cpu_arch::ARM32;
#elif defined(__powerpc__)
  return cpu_arch::kPowerPC;
#else
  static_assert(false, "get_cpu_arch::unknown cpu arch");
#endif
}

}  // namespace swiss

template <>
class std::formatter<swiss::cpu_arch> {
 public:
  template <typename FormatContext>
  constexpr auto parse(FormatContext & ctx) const noexcept -> decltype(auto) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(swiss::cpu_arch const & arch, FormatContext & ctx) const
      -> decltype(auto) {
    switch (arch) {
      case swiss::cpu_arch::kx86_64:
        return std::format_to(ctx.out(), "x86_64");
      case swiss::cpu_arch::kx86_32:
        return std::format_to(ctx.out(), "x86_32");
      case swiss::cpu_arch::kARM64:
        return std::format_to(ctx.out(), "ARM64");
      case swiss::cpu_arch::kARM32:
        return std::format_to(ctx.out(), "ARM32");
      case swiss::cpu_arch::kPowerPC:
        return std::format_to(ctx.out(), "PowerPC");
    }
  }
};
