#pragma once

#include <algorithm>
#include <array>

namespace swiss {

/**
 * @brief Fixed-size immutable sorted set
 */
template <typename T, std::size_t Size, typename Compare = std::less<T>>
class frozen_flat_set final {
 public:
  using value_type             = T;
  using reference              = T &;
  using const_reference        = T const &;
  using pointer                = T *;
  using const_pointer          = T const *;
  using key_compare            = Compare;
  using value_compare          = Compare;
  using iterator               = std::array<T, Size>::iterator;
  using const_iterator         = std::array<T, Size>::const_iterator;
  using reverse_iterator       = std::array<T, Size>::reverse_iterator;
  using const_reverse_iterator = std::array<T, Size>::const_reverse_iterator;
  using size_type              = std::array<T, Size>::size_type;
  using difference_type        = std::array<T, Size>::difference_type;

 public:
  // NOLINTNEXTLINE(google-explicit-constructor)
  constexpr frozen_flat_set(std::array<T, Size> keys,
                            Compare const & comp = Compare())
      : keys_(keys), comp_(comp) {
    std::sort(keys_.begin(), keys_.end(), comp_);
  }

 public:
  constexpr auto begin() noexcept -> iterator { return keys_.begin(); }
  constexpr auto begin() const noexcept -> const_iterator {
    return keys_.begin();
  }
  constexpr auto cbegin() const noexcept -> const_iterator {
    return keys_.cbegin();
  }

  constexpr auto end() noexcept -> iterator { return keys_.end(); }
  constexpr auto end() const noexcept -> const_iterator { return keys_.end(); }
  constexpr auto cend() const noexcept -> const_iterator {
    return keys_.cend();
  }

  constexpr auto rbegin() noexcept -> reverse_iterator {
    return keys_.rbegin();
  }
  constexpr auto rbegin() const noexcept -> const_reverse_iterator {
    return keys_.rbegin();
  }
  constexpr auto crbegin() const noexcept -> const_reverse_iterator {
    return keys_.crbegin();
  }

  constexpr auto rend() noexcept -> reverse_iterator { return keys_.rend(); }
  constexpr auto rend() const noexcept -> const_reverse_iterator {
    return keys_.rend();
  }
  constexpr auto crend() const noexcept -> const_reverse_iterator {
    return keys_.crend();
  }

 public:
  [[nodiscard]] constexpr auto empty() const noexcept -> bool {
    return Size == 0;
  }

  [[nodiscard]] constexpr auto size() const noexcept -> std::size_t {
    return Size;
  }

 public:
  constexpr auto operator[](std::size_t index) noexcept -> const_reference {
    return keys_[index];
  }

  constexpr auto operator[](std::size_t index) const noexcept
      -> const_reference {
    return keys_[index];
  }

 public:
  constexpr auto count(const_reference key) const noexcept -> std::size_t {
    return std::count(keys_.begin(), keys_.end(), key);
  }

  constexpr auto contains(const_reference key) const noexcept -> bool {
    return std::find(keys_.begin(), keys_.end(), key);
  }

 public:
  constexpr auto lower_bound(const_reference key) noexcept -> iterator {
    return std::lower_bound(keys_.begin(), keys_.end(), key, comp_);
  }

  constexpr auto upper_bound(const_reference key) noexcept -> iterator {
    return std::upper_bound(keys_.begin(), keys_.end(), key, comp_);
  }

  constexpr auto equal_range(const_reference key) noexcept
      -> std::pair<iterator, iterator> {
    return std::equal_range(keys_.begin(), keys_.end(), key, comp_);
  }

 private:
  std::array<T, Size> keys_;
  key_compare comp_;
};

}  // namespace swiss
