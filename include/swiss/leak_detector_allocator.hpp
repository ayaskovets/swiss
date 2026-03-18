#pragma once

#include <format>
#include <unordered_map>

namespace swiss {

/**
 * @brief Simple leak detecting allocator. If IsShared=true allocations are
 * stored on per-allocator-class basis rather than per-allocator-variable
 */
template <typename T, bool IsShared = false>
class leak_detector_allocator final : public std::allocator<T> {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = T &;
  using pointer = T *;
  using const_pointer = T const *;

 public:
  using std::allocator<T>::allocator;

 public:
  constexpr auto allocate(std::size_t const n) -> T * {
    T * const ptr = std::allocator<T>::allocate(n);

    auto const store_allocation_to = [ptr, n](auto & allocations) {
      if (allocations.count(ptr)) [[unlikely]] {
        throw std::runtime_error(
            std::format("double allocation on address {:p}",
                        static_cast<void const * const>(ptr)));
      }

      allocations.emplace(ptr, n);
    };

    if constexpr (IsShared) {
      store_allocation_to(shared_allocations_);
    } else {
      store_allocation_to(allocations_);
    }

    return ptr;
  }

  constexpr auto allocate_at_least(std::size_t const n)
      -> std::allocation_result<T *> {
    return {.ptr = allocate(n), .count = n};
  }

  constexpr auto deallocate(T * const ptr, std::size_t const n) -> void {
    auto const erase_allocation_from = [ptr, n](auto & allocations) {
      auto const allocation = allocations.find(ptr);
      if (allocation == allocations.end()) [[unlikely]] {
        throw std::runtime_error(
            std::format("deallocation on invalid address {:p}",
                        static_cast<void const * const>(ptr)));
      }

      auto const leaked_bytes =
          std::max(allocation->second, n) - std::min(allocation->second, n);
      if (leaked_bytes != 0) [[unlikely]] {
        throw std::runtime_error(
            std::format("leaked {} bytes on address {:p}", leaked_bytes,
                        static_cast<void const * const>(ptr)));
      }

      allocations.erase(allocation);
    };

    if constexpr (IsShared) {
      erase_allocation_from(shared_allocations_);
    } else {
      erase_allocation_from(allocations_);
    }

    std::allocator<T>::deallocate(ptr, n);
  }

 public:
  template <typename To>
  class rebind {
   public:
    using other = leak_detector_allocator<To, IsShared>;
  };

 public:
  constexpr auto clear() noexcept -> void {
    if constexpr (IsShared) {
      shared_allocations_.clear();
    } else {
      allocations_.clear();
    }
  }

 public:
  [[nodiscard]] constexpr auto empty() const noexcept -> bool {
    if constexpr (IsShared) {
      return shared_allocations_.empty();
    } else {
      return allocations_.empty();
    }
  }

 private:
  using allocations = std::unordered_map<void const *, std::size_t>;
  [[no_unique_address]] std::conditional_t<IsShared, std::monostate,
                                           allocations> allocations_;
  static inline allocations shared_allocations_;
};

}  // namespace swiss
