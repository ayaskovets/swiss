#include <swiss/overloaded.hpp>

#include <type_traits>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(overloaded, smoke) {
  auto const overloaded = swiss::overloaded{
      [](int value) -> int { return value; },
      [](std::string_view value) -> std::string_view { return value; }};

  static_assert(
      std::is_same_v<std::invoke_result_t<decltype(overloaded), int>, int>);
  static_assert(std::is_same_v<
                std::invoke_result_t<decltype(overloaded), std::string_view>,
                std::string_view>);
}

}  // namespace tests::unit