#include <swiss/chrono.hpp>

#include <thread>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(mocked_clock, smoke) {
  auto const now = swiss::mocked_clock<>::now();

  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  EXPECT_EQ(swiss::mocked_clock<>::now(), now);

  swiss::mocked_clock<>::shift(std::chrono::seconds(1));
  EXPECT_EQ(swiss::mocked_clock<>::now(), now + std::chrono::seconds(1));

  swiss::mocked_clock<>::set(now - std::chrono::seconds(1));
  EXPECT_EQ(swiss::mocked_clock<>::now(), now - std::chrono::seconds(1));
}

TEST(tls_mocked_clock, smoke) {
  auto const now = swiss::tls_mocked_clock<>::now();

  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  EXPECT_EQ(swiss::tls_mocked_clock<>::now(), now);

  swiss::tls_mocked_clock<>::shift(std::chrono::seconds(1));
  EXPECT_EQ(swiss::tls_mocked_clock<>::now(), now + std::chrono::seconds(1));

  swiss::tls_mocked_clock<>::set(now - std::chrono::seconds(1));
  EXPECT_EQ(swiss::tls_mocked_clock<>::now(), now - std::chrono::seconds(1));

  std::jthread const thread([now]() -> void {
    EXPECT_NE(swiss::tls_mocked_clock<>::now(), now - std::chrono::seconds(1));
  });
}

}  // namespace tests::unit
