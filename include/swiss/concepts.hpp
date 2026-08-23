#pragma once

#include <concepts>

namespace swiss {

template <typename T, typename... Args>
concept void_invocable = std::invocable<T, Args...> &&
                         std::is_same_v<std::invoke_result_t<T, Args...>, void>;

template <typename T, typename... Args>
concept nothrow_void_invocable =
    std::is_nothrow_invocable_v<T, Args...> &&
    std::is_same_v<std::invoke_result_t<T, Args...>, void>;

template <typename T, typename Other>
concept not_same_as = !std::same_as<T, Other>;

template <typename T, typename Other>
concept not_convertible_to = !std::convertible_to<T, Other>;

template <typename T, typename Other>
concept decay_same_as = std::same_as<std::decay_t<T>, Other>;

template <typename T, template <typename...> class Template>
concept instantiation_of = requires {
  []<typename... Args>(Template<Args...>) noexcept -> void {
  }(std::declval<std::decay_t<T>>());
};

template <typename T, typename... Types>
concept one_of = (std::same_as<T, Types> || ...);

}  // namespace swiss
