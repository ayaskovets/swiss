#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Type-erased pimpl with value semantics
 */
class unchecked_any final {
 public:
  template <typename T, typename... Args>
    requires(!std::is_array_v<T>)
  constexpr explicit unchecked_any(std::in_place_type_t<T>, Args &&... args)
    requires(std::constructible_from<T, Args...>)
      : delete_([](void * const ptr) {
          // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
          delete static_cast<T * const>(ptr);
        }),
        data_(new T(std::forward<Args>(args)...)),
        clone_([](void const * const ptr) -> void * {
          return std::make_unique<T>(*static_cast<T const *>(ptr)).release();
        }) {}

  template <typename T>
  constexpr explicit unchecked_any(T value)
      : unchecked_any(std::in_place_type<T>, std::move(value)) {}

  constexpr unchecked_any(unchecked_any const & that)
      : delete_(that.delete_),
        data_(that.clone_(that.data_)),
        clone_(that.clone_) {}

  constexpr auto operator=(unchecked_any const & that) noexcept(false)
      -> unchecked_any & {
    new (this) unchecked_any(that);
    return *this;
  }

  constexpr unchecked_any(unchecked_any && that) noexcept
      : delete_(std::exchange(that.delete_, nullptr)),
        data_(std::exchange(that.data_, nullptr)),
        clone_(std::exchange(that.clone_, nullptr)) {}

  constexpr auto operator=(unchecked_any && that) noexcept -> unchecked_any & {
    new (this) unchecked_any(std::move(that));
    return *this;
  }

  constexpr ~unchecked_any() noexcept {
    if (!valueless_after_move()) {
      delete_(data_);
    }
  }

 public:
  template <typename T>
  constexpr auto operator=(T value) -> unchecked_any & {
    new (this) unchecked_any(std::move(value));
    return *this;
  }

 public:
  [[nodiscard]] constexpr auto valueless_after_move() const noexcept -> bool {
    return delete_ == nullptr;
  }

 public:
  template <typename T>
  constexpr auto get() noexcept -> T & {
    return *static_cast<T *>(data_);
  }

  template <typename T>
  constexpr auto get() const noexcept -> T const & {
    return *static_cast<T *>(data_);
  }

  template <typename T>
  constexpr auto value() const noexcept -> T
    requires std::is_copy_constructible_v<T>
  {
    return *static_cast<T *>(data_);
  }

 private:
  using deleter_type = void (*)(void *);
  deleter_type delete_;

  void * data_;

  using cloner_type = void * (*)(void const *);
  cloner_type clone_;
};

}  // namespace swiss
