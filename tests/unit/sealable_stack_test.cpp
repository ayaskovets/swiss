#include <swiss/sealable_stack.hpp>

#include <iterator>
#include <ranges>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(sealable_stack, concepts) {
  using stack_type = swiss::sealable_stack<int>;

  static_assert(std::ranges::forward_range<stack_type>);
  static_assert(std::forward_iterator<stack_type::iterator>);
  static_assert(std::ranges::forward_range<stack_type const>);
  static_assert(std::forward_iterator<stack_type::const_iterator>);
}

TEST(sealable_stack, smoke) {
  using stack_type = swiss::sealable_stack<int>;

  stack_type stack;
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.top(), nullptr);
  EXPECT_FALSE(stack.sealed());

  stack_type::node const node(1);
  EXPECT_TRUE(stack.try_push(node));
  EXPECT_EQ(stack.top(), &node);
  EXPECT_FALSE(stack.sealed());
  EXPECT_FALSE(stack.empty());

  stack.seal();
  EXPECT_FALSE(stack.try_push(node));
  EXPECT_EQ(stack.top(), &node);
  EXPECT_TRUE(stack.sealed());
  EXPECT_FALSE(stack.empty());
}

TEST(sealable_stack, non_copyable_item_type) {
  using stack_type = swiss::sealable_stack<std::unique_ptr<int>>;

  stack_type stack;
  stack_type::node const node(std::make_unique<int>(1));

  EXPECT_TRUE(stack.try_push(node));
  EXPECT_EQ(stack.top(), &node);
}

TEST(sealable_stack, iterator) {
  using stack_type = swiss::sealable_stack<int>;

  stack_type stack;
  stack_type::node const node1(1);
  stack_type::node const node2(2);
  stack_type::node const node3(3);

  EXPECT_TRUE(stack.try_push(node1));
  EXPECT_TRUE(stack.try_push(node2));
  EXPECT_TRUE(stack.try_push(node3));

  std::vector<int> values;
  for (int const value : stack) {
    values.push_back(value);
  }

  EXPECT_EQ(values, (std::vector<int>{3, 2, 1}));
}

}  // namespace tests::unit
