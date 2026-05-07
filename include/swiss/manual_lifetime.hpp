#pragma once

namespace swiss {

/**
 * @brief A union wrapper to decouple object lifetime from its storage
 * @note Uses pointer-like syntax for access to the implementation
 * @warning Lifetime of the underlying object must be managed by the user of
 * this class
 */
template <typename T>
class manual_lifetime {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  constexpr auto data() noexcept -> pointer { return &storage_.value; }
  constexpr auto data() const noexcept -> const_pointer {
    return &storage_.value;
  }

 public:
  constexpr auto operator*() noexcept -> T & { return storage_.value; }
  constexpr auto operator*() const noexcept -> T const & {
    return storage_.value;
  }
  constexpr auto operator->() noexcept -> T * { return &storage_.value; }
  constexpr auto operator->() const noexcept -> T const * {
    return &storage_.value;
  }

 private:
  union Storage final {
    T value;

    constexpr Storage() noexcept {}
    constexpr Storage(Storage const &) noexcept        = delete;
    constexpr Storage(Storage &&) noexcept             = delete;
    constexpr auto operator=(Storage const &) noexcept = delete;
    constexpr auto operator=(Storage &&) noexcept      = delete;
    constexpr ~Storage() noexcept {}
  } storage_;
};

}  // namespace swiss
