#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Type-erased pimpl with value semantics
 * @note Noexcept if the copy allocation does not throw exceptions
 * @warning This type is different from std::any in that it does not check that
 * the requested type is the same as stored and subsequently does not throw any
 * exceptions. Converting the stored memory to an invalid type is UB
 */
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class unchecked_any {
 public:
  constexpr unchecked_any() noexcept
      : delete_(nullptr), data_(nullptr), clone_(nullptr) {}

  template <typename T>
  explicit constexpr unchecked_any(T value) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : unchecked_any(std::in_place_type<T>, std::move(value)) {}

  template <typename T, typename... Args>
    requires(!std::is_array_v<T>)
  explicit constexpr unchecked_any(
      std::in_place_type_t<T>,
      Args &&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    requires(std::constructible_from<T, Args...>)
      : delete_([](void * ptr) noexcept -> void {
          // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
          delete static_cast<T *>(ptr);
        }),
        data_(new T(std::forward<Args>(args)...)),
        clone_([](void const * const ptr) -> void * {
          return std::make_unique<T>(*static_cast<T const *>(ptr)).release();
        }) {}

  constexpr unchecked_any(unchecked_any const & that)
      : delete_(that.delete_),
        data_((that.clone_ != nullptr) ? that.clone_(that.data_) : nullptr),
        clone_(that.clone_) {}

  constexpr unchecked_any(unchecked_any && that) noexcept
      : delete_(std::exchange(that.delete_, nullptr)),
        data_(std::exchange(that.data_, nullptr)),
        clone_(std::exchange(that.clone_, nullptr)) {}

  constexpr auto operator=(unchecked_any that) -> unchecked_any & {
    std::swap(this->delete_, that.delete_);
    std::swap(this->data_, that.data_);
    std::swap(this->clone_, that.clone_);
    return *this;
  }

  constexpr ~unchecked_any() noexcept {
    if (has_value()) {
      delete_(data_);
    }
  }

 public:
  template <typename T>
  constexpr auto operator=(T value) noexcept(
      std::is_nothrow_move_constructible_v<T>) -> unchecked_any & {
    *this = unchecked_any(std::move(value));
    return *this;
  }

 public:
  [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
    return delete_ != nullptr;
  }

 public:
  template <typename T>
  constexpr auto get() noexcept -> T * {
    return static_cast<T *>(data_);
  }

  template <typename T>
  constexpr auto get() const noexcept -> T const * {
    return static_cast<T const *>(data_);
  }

  template <typename T>
  constexpr auto value() const noexcept -> T
    requires std::is_copy_constructible_v<T>
  {
    return *static_cast<T const *>(data_);
  }

 private:
  using deleter_type = void (*)(void *);
  deleter_type delete_;

  void * data_;

  using cloner_type = void * (*)(void const *);
  cloner_type clone_;
};

}  // namespace swiss
