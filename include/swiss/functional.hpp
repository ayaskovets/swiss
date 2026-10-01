#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace swiss {

/**
 * @brief Invoke a callable with all parameters in pack except the first one
 */
template <typename Head, typename... Tail>
constexpr auto invoke_tail(
    std::invocable<Tail...> auto && invocable, Head &&,
    Tail &&... tail) noexcept(std::is_nothrow_invocable_v<decltype(invocable),
                                                          Tail...>)
    -> decltype(auto) {
  return std::invoke(std::forward<decltype(invocable)>(invocable),
                     std::forward<Tail>(tail)...);
}

/**
 * @brief Invoke a class method with all parameters in pack except the first one
 */
template <typename Head, typename... Tail>
constexpr auto invoke_tail(
    auto && _this, auto const & method, Head &&,
    Tail &&... tail) noexcept(std::is_nothrow_invocable_v<decltype(method),
                                                          decltype(_this),
                                                          Tail...>)
    -> decltype(auto)
  requires(std::invocable<decltype(method), decltype(_this), Tail...>)
{
  return std::invoke(method, std::forward<decltype(_this)>(_this),
                     std::forward<Tail>(tail)...);
}

/**
 * @brief Invoke a function and assign its return value to the first argument if
 * the function return type is not void
 */
template <typename T, typename Callable, typename... Args>
constexpr void assign_invoke_result_if_not_void(
    T & ret, Callable && callable,
    Args &&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
  if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
    std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
  } else {
    ret = std::invoke(std::forward<Callable>(callable),
                      std::forward<Args>(args)...);
  }
}

/**
 * @brief Poor man's std::move_only_function replacement. Wraps a callable that
 * does not throw
 */
template <typename Ret, typename... Args>
class noexcept_function : public std::function<Ret(Args...)> {
 private:
  using base = std::function<Ret(Args...)>;

 public:
  constexpr noexcept_function()                     = delete;
  constexpr noexcept_function(noexcept_function &&) = delete;
  constexpr auto operator=(noexcept_function &&)
      -> noexcept_function & = delete;

 public:
  constexpr noexcept_function(noexcept_function const &) = default;
  constexpr auto operator=(noexcept_function const &)
      -> noexcept_function & = default;

 public:
  template <std::invocable<Args...> Callable>
  // NOLINTNEXTLINE(google-explicit-constructor)
  constexpr noexcept_function(Callable && callable)
    requires(std::is_nothrow_invocable_v<Callable, Args...> &&
             !std::same_as<std::decay_t<decltype(callable)>, noexcept_function>)
      : base(std::forward<Callable>(callable)) {
    if (!base::operator bool()) {
      throw std::invalid_argument(
          "noexcept_function::noexcept_function(): callable must not be null");
    }
  }

  constexpr ~noexcept_function() = default;

 public:
  constexpr auto operator=(auto && callable) -> noexcept_function &
    requires(!std::same_as<std::decay_t<decltype(callable)>, noexcept_function>)
  {
    base const that(std::forward<decltype(callable)>(callable));

    if (!that) {
      throw std::invalid_argument(
          "noexcept_function::operator=(): callable must not be null");
    }

    base::operator=(std::move(that));
    return *this;
  }

 public:
  auto operator()(auto &&... args) const noexcept -> Ret {
    return base::operator()(std::forward<decltype(args)>(args)...);
  }
};

/**
 * @brief Wrapped member function call with a set object instance captured by
 * reference
 */
template <typename T, typename Ret, typename... Args>
class memfun final {
 public:
  constexpr memfun() noexcept = default;
  constexpr memfun(T & object, Ret (T::*memfun)(Args...))
      : object_(&object), memfun_(memfun) {
    if (memfun_ == nullptr) {
      throw std::invalid_argument("memfun::memfun(): null pointer to member");
    }
  }

 public:
  explicit constexpr operator bool() const noexcept { return memfun_; }

  constexpr auto operator()(auto &&... args) const -> Ret {
    if (!memfun_) [[unlikely]] {
      throw std::runtime_error("memfun::operator()(): null pointer to member");
    }

    return (object_->*memfun_)(std::forward<decltype(args)>(args)...);
  }

 private:
  T * object_;
  Ret (T::*memfun_)(Args...){};
};

template <typename T, typename Ret, typename... Args>
class const_memfun final {
 public:
  constexpr const_memfun() noexcept = default;
  constexpr const_memfun(T const & object, Ret (T::*memfun)(Args...) const)
      : object_(&object), memfun_(memfun) {
    if (memfun_ == nullptr) {
      throw std::invalid_argument(
          "const_memfun::const_memfun(): null pointer to member");
    }
  }

 public:
  explicit constexpr operator bool() const noexcept { return memfun_; }

  constexpr auto operator()(auto &&... args) const -> Ret {
    if (!memfun_) [[unlikely]] {
      throw std::runtime_error("memfun::operator()(): null pointer to member");
    }

    return (object_->*memfun_)(std::forward<decltype(args)>(args)...);
  }

 private:
  T const * object_;
  Ret (T::*memfun_)(Args...) const {};
};

}  // namespace swiss
