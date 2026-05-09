#pragma once

#include <array>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Pimpl without dynamic allocations. Implementation is stored in a
 * buffer allocated on the stack
 * @note Uses pointer-like syntax for access to the implementation
 * @note Size and alignment must be manually set for the specific instance
 */
template <typename T, std::size_t Size, std::size_t Alignment>
class static_pimpl {
 public:
  using value_type      = T;
  using reference       = T &;
  using const_reference = T &;
  using pointer         = T *;
  using const_pointer   = T const *;

 public:
  template <typename... Args>
    requires(std::constructible_from<T, Args...>)
  explicit constexpr static_pimpl(Args &&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>) {
    std::construct_at(operator->(), std::forward<Args>(args)...);
  }

  constexpr static_pimpl(static_pimpl const & that) noexcept(
      std::is_nothrow_copy_constructible_v<T>) {
    std::construct_at(operator->(), that.operator*());
  }

  constexpr ~static_pimpl() noexcept {
    static_assert(sizeof(T) == Size);
    static_assert(alignof(T) == Alignment);
    std::destroy_at(operator->());
  }

 public:
  // NOLINTNEXTLINE(cert-oop54-cpp)
  constexpr auto operator=(static_pimpl const & that) noexcept(
      std::is_nothrow_copy_assignable_v<T>) -> static_pimpl & {
    operator*() = that.operator*();
    return *this;
  }

  constexpr auto operator=(T const & that) noexcept(
      std::is_nothrow_copy_assignable_v<T>) -> static_pimpl & {
    operator*() = that;
    return *this;
  }

  constexpr static_pimpl(static_pimpl && that) noexcept(
      std::is_nothrow_move_constructible_v<T>) {
    std::construct_at(operator->(), std::move(that.operator*()));
  }

  constexpr auto operator=(static_pimpl && that) noexcept(
      std::is_nothrow_move_assignable_v<T>) -> static_pimpl & {
    operator*() = std::move(that.operator*());
    return *this;
  }

  constexpr auto operator=(T && that) noexcept(
      std::is_nothrow_move_assignable_v<T>) -> static_pimpl & {
    operator*() = std::move(that);
    return *this;
  }

 public:
  constexpr auto operator<=>(static_pimpl const & that) const noexcept -> auto {
    return operator*() <=> that.operator*();
  }

  constexpr auto operator==(static_pimpl const & that) const noexcept -> bool {
    return operator*() == that.operator*();
  }

 public:
  constexpr auto operator*() noexcept -> T & {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *std::launder(reinterpret_cast<T *>(impl_.data()));
  }
  constexpr auto operator*() const noexcept -> T const & {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *std::launder(reinterpret_cast<T const *>(impl_.data()));
  }

  constexpr auto operator->() noexcept -> T * {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return std::launder(reinterpret_cast<T *>(impl_.data()));
  }
  constexpr auto operator->() const noexcept -> T const * {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return std::launder(reinterpret_cast<T const *>(impl_.data()));
  }

 private:
  alignas(Alignment) std::array<std::byte, Size> impl_;
};

}  // namespace swiss
