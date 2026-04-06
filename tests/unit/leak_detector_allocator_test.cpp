#include <swiss/leak_detector_allocator.hpp>

#include <gtest/gtest.h>

namespace tests::unit {

TEST(leak_detector_allocator, size) {
  static_assert(sizeof(swiss::leak_detector_allocator<int>) ==
                sizeof(std::unordered_map<int, int>));
  static_assert(sizeof(swiss::leak_detector_allocator<int>) ==
                sizeof(std::unordered_map<int, int>));

  static_assert(sizeof(swiss::leak_detector_allocator<int, true>) == 1);
  static_assert(sizeof(swiss::leak_detector_allocator<int, true>) == 1);
}

TEST(leak_detector_allocator, allocate) {
  constexpr std::size_t kAllocatedBlocks = 3;

  swiss::leak_detector_allocator<int> allocator;
  EXPECT_TRUE(allocator.empty());

  int * const ptr = allocator.allocate(kAllocatedBlocks);
  EXPECT_FALSE(allocator.empty());

  EXPECT_ANY_THROW(allocator.deallocate(nullptr, kAllocatedBlocks));
  EXPECT_ANY_THROW(allocator.deallocate(ptr, kAllocatedBlocks + 1));
  EXPECT_NO_THROW(allocator.deallocate(ptr, kAllocatedBlocks));
  EXPECT_TRUE(allocator.empty());
}

TEST(leak_detector_allocator, allocate_at_least) {
  constexpr std::size_t kAllocatedBlocks = 3;

  swiss::leak_detector_allocator<int> allocator;
  EXPECT_TRUE(allocator.empty());

  auto const result = allocator.allocate_at_least(kAllocatedBlocks);
  EXPECT_FALSE(allocator.empty());

  EXPECT_ANY_THROW(allocator.deallocate(nullptr, kAllocatedBlocks));
  EXPECT_ANY_THROW(allocator.deallocate(result.ptr, kAllocatedBlocks + 1));
  EXPECT_NO_THROW(allocator.deallocate(result.ptr, result.count));
  EXPECT_TRUE(allocator.empty());
}

TEST(leak_detector_allocator, clear) {
  constexpr std::size_t kAllocatedBlocks = 3;

  swiss::leak_detector_allocator<int> allocator;
  allocator.allocate(kAllocatedBlocks);
  EXPECT_FALSE(allocator.empty());

  allocator.clear();
  EXPECT_TRUE(allocator.empty());
}

TEST(leak_detector_allocator, shared) {
  swiss::leak_detector_allocator<int, true> allocator;

  {
    std::vector<int, decltype(allocator)> vector(allocator);
    EXPECT_TRUE(allocator.empty());

    vector.push_back(1);
    EXPECT_FALSE(allocator.empty());

    decltype(allocator) const shared_copy;
    EXPECT_FALSE(shared_copy.empty());

    swiss::leak_detector_allocator<int> const copy;
    EXPECT_TRUE(copy.empty());
  }

  EXPECT_TRUE(allocator.empty());
}

}  // namespace tests::unit
