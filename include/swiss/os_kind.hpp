#pragma once

#include <cstdint>
#include <format>

namespace swiss {

enum class os_kind : std::uint8_t {
  kWindows,
  kMacOS,
  kLinux,
  kFreeBSD,
  kUnix,
};

constexpr auto get_os_kind() noexcept -> os_kind {
#ifdef _WIN32
  return os_kind::kWindows;
#elif __APPLE__ || __MACH__
  return os_kind::kMacOS;
#elif __linux__
  return os_kind::kLinux;
#elif __FreeBSD__
  return os_kind::kFreeBSD;
#elif __unix || __unix__
  return os_kind::kUnix;
#else
  static_assert(false, "get_os_kind::unknown os");
#endif
}

}  // namespace swiss

template <>
class std::formatter<swiss::os_kind> {
 public:
  template <typename FormatContext>
  constexpr auto parse(FormatContext & ctx) const noexcept -> decltype(auto) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(swiss::os_kind const & kind, FormatContext & ctx) const
      -> decltype(auto) {
    switch (kind) {
      case swiss::os_kind::kWindows:
        return std::format_to(ctx.out(), "Windows");
      case swiss::os_kind::kMacOS:
        return std::format_to(ctx.out(), "macOS");
      case swiss::os_kind::kLinux:
        return std::format_to(ctx.out(), "Linux");
      case swiss::os_kind::kFreeBSD:
        return std::format_to(ctx.out(), "FreeBSD");
      case swiss::os_kind::kUnix:
        return std::format_to(ctx.out(), "Unix");
    }
  }
};
