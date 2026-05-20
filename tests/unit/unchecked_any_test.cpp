#include <swiss/unchecked_any.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(unchecked_any, size) {
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  // NOLINTBEGIN(readability-magic-numbers)
  static_assert(sizeof(swiss::unchecked_any) == 24);
  static_assert(alignof(swiss::unchecked_any) == 8);
  // NOLINTEND(readability-magic-numbers)
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(unchecked_any, inplace_construction) {
  std::string const kSomeString("a string");

  swiss::unchecked_any const value(std::in_place_type<std::string>,
                                   kSomeString);
  EXPECT_EQ(value.value<std::string>(), kSomeString);
  EXPECT_EQ(value.get<std::string>()->size(), kSomeString.size());
}

TEST(unchecked_any, construction) {
  std::string const kSomeString("a string");

  swiss::unchecked_any value(kSomeString);
  EXPECT_EQ(value.value<std::string>(), kSomeString);
  EXPECT_EQ(value.get<std::string>()->size(), kSomeString.size());

  value = std::string(kSomeString);
  EXPECT_EQ(value.value<std::string>(), kSomeString);
  EXPECT_EQ(value.get<std::string>()->size(), kSomeString.size());
}

TEST(unchecked_any, destruction) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::unchecked_any const pimpl(ptr);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*pimpl.get<decltype(ptr)>(), ptr);
    EXPECT_EQ(pimpl.value<decltype(ptr)>(), ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(unchecked_any, copy_constructor) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::unchecked_any const pimpl(ptr);
    EXPECT_EQ(ptr.use_count(), 2);

    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    swiss::unchecked_any const copy(pimpl);
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(*pimpl.get<decltype(ptr)>(), ptr);
    EXPECT_EQ(*pimpl.get<decltype(ptr)>(), *copy.get<decltype(ptr)>());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(unchecked_any, copy_assignment) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::unchecked_any const pimpl(ptr);
    swiss::unchecked_any copy(2);
    copy = pimpl;
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(*pimpl.get<decltype(ptr)>(), ptr);
    EXPECT_EQ(*pimpl.get<decltype(ptr)>(), *copy.get<decltype(ptr)>());
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(unchecked_any, move_constructor) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::unchecked_any pimpl(ptr);
    EXPECT_TRUE(pimpl.has_value());

    swiss::unchecked_any const moved_to(std::move(pimpl));
    EXPECT_FALSE(pimpl.has_value());
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*moved_to.get<decltype(ptr)>(), ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(unchecked_any, move_assignment) {
  auto ptr = std::make_shared<int>(1);
  {
    swiss::unchecked_any pimpl(ptr);
    EXPECT_TRUE(pimpl.has_value());

    swiss::unchecked_any moved_to(2);
    moved_to = std::move(pimpl);
    EXPECT_FALSE(pimpl.has_value());
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*moved_to.get<decltype(ptr)>(), ptr);
  }
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(unchecked_any, value_assignment) {
  std::string const kSomeString    = "a string";
  std::string const kAnotherString = "another string";

  swiss::unchecked_any value(kSomeString);
  EXPECT_EQ(*value.get<std::string>(), kSomeString);

  *value.get<std::string>() = kAnotherString;
  EXPECT_EQ(*value.get<std::string>(), kAnotherString);

  *value.get<std::string>() = kAnotherString;
  EXPECT_EQ(*value.get<std::string>(), kAnotherString);
}

}  // namespace tests::unit
