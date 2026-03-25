#include <swiss/strong_typedef.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(strong_typedef, size) {
  static_assert(sizeof(swiss::strong_typedef<std::monostate, std::string>) ==
                sizeof(std::string));
  static_assert(alignof(swiss::strong_typedef<std::monostate, std::string>) ==
                alignof(std::string));
}

TEST(strong_typedef, smoke) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::strong_typedef<std::monostate, decltype(ptr)> const value(ptr);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*value, ptr);
    EXPECT_EQ((*value).get(), ptr.get());
    EXPECT_EQ(value->get(), ptr.get());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(strong_typedef, user_defined_type) {
  constexpr auto kValue = 42;

  swiss::strong_typedef<class tag, int> const value(kValue);
  EXPECT_EQ(*value, kValue);
}

TEST(strong_typedef, copy_constructor) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::strong_typedef<std::monostate, decltype(ptr)> const value(ptr);
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    auto const copy(value);
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(value, copy);
    EXPECT_EQ(value->get(), ptr.get());
    EXPECT_EQ(value->get(), copy->get());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(strong_typedef, copy_assignment) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::strong_typedef<std::monostate, decltype(ptr)> const value(ptr);
    std::remove_const_t<decltype(value)> copy;
    copy = value;
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(value, copy);
    EXPECT_EQ(value->get(), ptr.get());
    EXPECT_EQ(value->get(), copy->get());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(strong_typedef, move_constructor) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::strong_typedef<std::monostate, std::shared_ptr<int>> value(ptr);
    auto const move(std::move(value));
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*move, ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(strong_typedef, move_assignment) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::strong_typedef<std::monostate, std::shared_ptr<int>> value(ptr);
    std::remove_const_t<decltype(value)> move;
    move = std::move(value);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*move, ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(strong_typedef, value_assignment) {
  std::string const kSomeString    = "a string";
  std::string const kAnotherString = "another string";

  swiss::strong_typedef<std::monostate, std::string> value(kSomeString);
  EXPECT_EQ(*value, kSomeString);

  *value = kAnotherString;
  EXPECT_EQ(*value, kAnotherString);

  *value = std::string(kAnotherString);
  EXPECT_EQ(*value, kAnotherString);
}

}  // namespace tests::unit
