#include <swiss/scope_fail.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(scope_fail, nothrow_destructor) {
  bool flag    = false;
  auto finally = [&flag]() noexcept -> void { flag = true; };

  {
    // NOLINTNEXTLINE(readability-identifier-length)
    swiss::scope_fail const _(std::move(finally));
    EXPECT_FALSE(flag);
  }

  EXPECT_FALSE(flag);
}

TEST(scope_fail, throw_destructor) {
  bool flag    = false;
  auto finally = [&flag]() noexcept -> void { flag = true; };

  try {
    // NOLINTNEXTLINE(readability-identifier-length)
    swiss::scope_fail const _(std::move(finally));
    EXPECT_FALSE(flag);

    throw std::runtime_error("");

    // NOLINTNEXTLINE(bugprone-empty-catch)
  } catch (...) {
  }

  EXPECT_TRUE(flag);
}

}  // namespace tests::unit
