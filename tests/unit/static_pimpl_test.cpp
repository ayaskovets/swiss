#include <swiss/static_pimpl.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(static_pimpl, size) {
  class alignas(2) impl final {};

  static_assert(sizeof(swiss::static_pimpl<impl, 2, 2>) == sizeof(impl));
  static_assert(alignof(swiss::static_pimpl<impl, 2, 2>) == alignof(impl));
}

TEST(static_pimpl, smoke) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::static_pimpl<decltype(ptr), sizeof(ptr),
                        alignof(decltype(ptr))> const value(ptr);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*value, ptr);
    EXPECT_EQ((*value).get(), ptr.get());
    EXPECT_EQ(value->get(), ptr.get());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(static_pimpl, copy_constructor) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::static_pimpl<decltype(ptr), sizeof(ptr),
                        alignof(decltype(ptr))> const value(ptr);
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    auto const copy(value);
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(value, copy);
    EXPECT_EQ(value->get(), ptr.get());
    EXPECT_EQ(value->get(), copy->get());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(static_pimpl, copy_assignment) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::static_pimpl<decltype(ptr), sizeof(ptr),
                        alignof(decltype(ptr))> const value(ptr);
    std::remove_const_t<decltype(value)> copy;
    copy = value;
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(value, copy);
    EXPECT_EQ(value->get(), ptr.get());
    EXPECT_EQ(value->get(), copy->get());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(static_pimpl, move_constructor) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::static_pimpl<decltype(ptr), sizeof(ptr), alignof(decltype(ptr))>
        value(ptr);
    auto const move(std::move(value));
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*move, ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(static_pimpl, move_assignment) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::static_pimpl<decltype(ptr), sizeof(ptr), alignof(decltype(ptr))>
        value(ptr);
    std::remove_const_t<decltype(value)> move;
    move = std::move(value);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*move, ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(static_pimpl, value_assignment) {
  std::string const kSomeString = "a string";
  std::string const kAnotherString = "another string";

  swiss::static_pimpl<std::string, sizeof(std::string), alignof(std::string)>
      value(kSomeString);
  EXPECT_EQ(*value, kSomeString);

  *value = kAnotherString;
  EXPECT_EQ(*value, kAnotherString);

  *value = std::string(kAnotherString);
  EXPECT_EQ(*value, kAnotherString);
}

}  // namespace tests::unit
