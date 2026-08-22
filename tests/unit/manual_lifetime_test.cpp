#include <swiss/manual_lifetime.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(manual_lifetime, size) {
  static_assert(sizeof(swiss::manual_lifetime<std::string>) ==
                sizeof(std::string));
  static_assert(alignof(swiss::manual_lifetime<std::string>) ==
                alignof(std::string));
}

namespace {

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
struct StaticCounter final {
  constexpr StaticCounter() noexcept { counter += 1; };
  constexpr ~StaticCounter() noexcept { counter -= 1; };

  static inline int counter = 0;
};

TEST(manual_lifetime, lifetime_leak) {
  EXPECT_EQ(StaticCounter::counter, 0);

  {
    StaticCounter counter;
    EXPECT_EQ(StaticCounter::counter, 1);

    {
      swiss::manual_lifetime<StaticCounter> value;
      EXPECT_EQ(StaticCounter::counter, 1);
      std::construct_at(value.operator->());
      EXPECT_EQ(StaticCounter::counter, 2);
    }

    EXPECT_EQ(StaticCounter::counter, 2);

    {
      swiss::manual_lifetime<StaticCounter> value;
      EXPECT_EQ(StaticCounter::counter, 2);
      std::construct_at(value.operator->());
      EXPECT_EQ(StaticCounter::counter, 3);
      std::destroy_at(value.operator->());
      EXPECT_EQ(StaticCounter::counter, 2);
    }
  }

  EXPECT_EQ(StaticCounter::counter, 1);
}

}  // namespace

}  // namespace tests::unit
