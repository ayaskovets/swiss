#include <swiss/scope_exit.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(scope_exit, destructor) {
  bool flag    = false;
  auto finally = [&flag]() noexcept -> void { flag = true; };

  {
    // NOLINTNEXTLINE(readability-identifier-length)
    swiss::scope_exit const _(std::move(finally));
    EXPECT_FALSE(flag);
  }

  EXPECT_TRUE(flag);
}

}  // namespace tests::unit
