#pragma once

#include <concepts>
#include <utility>
#include <variant>

namespace swiss {

/**
 * @brief Invocable returning void
 */
template <typename T, typename... Args>
concept void_invocable = std::invocable<T, Args...> &&
                         std::is_same_v<std::invoke_result_t<T, Args...>, void>;

/**
 * @brief Invocable returning void that does not throw
 */
template <typename T, typename... Args>
concept nothrow_void_invocable =
    std::is_nothrow_invocable_v<T, Args...> &&
    std::is_same_v<std::invoke_result_t<T, Args...>, void>;

/**
 * @brief Inverse of std::same_as
 */
template <typename T, typename Other>
concept not_same_as = !std::same_as<T, Other>;

/**
 * @brief Inverse of std::convertible_to
 */
template <typename T, typename Other>
concept not_convertible_to = !std::convertible_to<T, Other>;

/**
 * @brief Check that the decayed type is the same as the second parameter
 */
template <typename T, typename Other>
concept decay_same_as = std::same_as<std::decay_t<T>, Other>;

/**
 * @brief Check that the first parameter in an instantiation of some template
 */
template <typename T, template <typename...> class Template>
concept instantiation_of = requires {
  []<typename... Args>(Template<Args...>) noexcept -> void {
  }(std::declval<std::decay_t<T>>());
};

/**
 * @brief Check that type belongs to the parameter pack
 */
template <typename T, typename... Types>
concept one_of = (std::same_as<T, Types> || ...);

/**
 * @brief Return the result of co_await operator on the type if there is any
 * otherwise return the value itself
 */
template <typename T>
constexpr auto co_await_result_v(T && awaiter) -> decltype(auto) {
  if constexpr (requires { std::forward<T>(awaiter).operator co_await(); }) {
    return std::forward<T>(awaiter).operator co_await();
  } else if constexpr (requires {
                         operator co_await(std::forward<T>(awaiter));
                       }) {
    return operator co_await(std::forward<T>(awaiter));
  } else {
    return std::forward<T>(awaiter);
  }
}

/**
 * @brief Return decayed type of the co_await operator resulting type
 */
template <typename T>
using co_await_result_t =
    std::decay_t<decltype(co_await_result_v(std::declval<T>()))>;

template <typename Needle, typename... Haystack>
constexpr std::size_t count_in_pack =
    []() -> std::size_t { return (std::is_same_v<Needle, Haystack> + ...); }();

template <typename Needle, typename... Haystack>
constexpr std::size_t index_in_pack = []() -> std::size_t {
  static_assert((std::is_same_v<Needle, Haystack> || ...),
                "index_in_pack: not found");

  std::array const matches{std::is_same_v<Needle, Haystack>...};
  for (std::size_t i = 0; i < sizeof...(Haystack); ++i) {
    if (matches[i]) {
      return i;
    }
  }

  std::unreachable();
}();

/**
 * @brief Check that types in parameter pack are unique
 */
template <typename... Ts>
constexpr bool is_unique_pack =
    (([]<typename Needle, typename... Haystack>() -> bool {
       return (std::is_same_v<Needle, Haystack> + ...) == 1;
     }.template operator()<Ts, Ts...>()) &&
     ...);

/**
 * @brief Standard variant with unique types
 */
template <typename T>
concept unique_variant = requires {
  requires([]<typename... Args>(std::variant<Args...> *) -> bool {
    return is_unique_pack<Args...>;
  }(static_cast<T *>(nullptr)));
};

}  // namespace swiss
