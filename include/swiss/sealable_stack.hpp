#pragma once

#include <atomic>
#include <optional>
#include <type_traits>
#include <utility>

namespace swiss {

/**
 * @brief Thread-safe stack without the pop operation
 * @note One-time sealable i.e. at some point of execution push operations could
 * be prohibited
 * @note Nodes are stored on the call site. Stack does not own it's nodes
 * @warning Does not prevent cycles. If a node occurs multiple times in stack
 * iteration will block indefinitely
 */
template <typename T>
class sealable_stack final {
 public:
  class node;
  using value_type      = T;
  using reference       = T &;
  using const_reference = T const &;
  using pointer         = T *;
  using const_pointer   = T const *;
  class const_iterator;
  using iterator        = const_iterator;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;

 public:
  constexpr sealable_stack() noexcept : top_(nullptr) {}

 public:
  constexpr auto begin() noexcept -> iterator { return iterator(top()); }
  constexpr auto begin() const noexcept -> const_iterator {
    return const_iterator(top());
  }
  constexpr auto cbegin() const noexcept -> const_iterator {
    return const_iterator(top());
  }

  constexpr auto end() noexcept -> iterator { return {}; }
  constexpr auto end() const noexcept -> const_iterator { return {}; }
  constexpr auto cend() const noexcept -> const_iterator { return {}; }

 public:
  /**
   * @note Nodes are allocated on the call site
   */
  constexpr auto try_push(node && node) noexcept -> bool = delete;

  [[nodiscard]] constexpr auto try_push(node const & node) noexcept -> bool {
    class node const * expected = top_.load(std::memory_order::acquire);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-do-while)
    do {
      if (expected && expected->is_seal()) {
        return false;
      }

      node.next_ = expected;
    } while (!top_.compare_exchange_weak(expected, &node,
                                         std::memory_order::acquire,
                                         std::memory_order::relaxed));
    return true;
  }

  constexpr void seal() noexcept {
    node const * expected = top_.load(std::memory_order::acquire);

    thread_local node seal;
    seal = node::kSeal(expected);

    while (!top_.compare_exchange_weak(expected, &seal,
                                       std::memory_order::acquire,
                                       std::memory_order::relaxed)) {
    }
  }

 public:
  constexpr auto top() const noexcept -> node const * {
    node const * const top = top_.load(std::memory_order::acquire);
    return (top && top->is_seal()) ? top->next_ : top;
  }

  [[nodiscard]] constexpr auto empty() const noexcept -> bool { return !top(); }

  [[nodiscard]] constexpr auto sealed() const noexcept -> bool {
    node const * const top = top_.load(std::memory_order::acquire);
    return (top && top->is_seal());
  }

 private:
  std::atomic<node const *> top_;
};

template <typename T>
class sealable_stack<T>::const_iterator final {
 public:
  using value_type      = T;
  using reference       = T const &;
  using pointer         = T const *;
  using const_pointer   = T const *;
  using const_reference = T const &;
  using difference_type = std::ptrdiff_t;

 public:
  constexpr const_iterator() noexcept : node_(nullptr) {}
  explicit constexpr const_iterator(node const * node) noexcept : node_(node) {}

 public:
  constexpr auto operator==(const_iterator const & that) const noexcept
      -> bool = default;

  constexpr auto operator*() noexcept -> const_reference {
    return node_->value();
  }
  constexpr auto operator*() const noexcept -> const_reference {
    return node_->value();
  }

  constexpr auto operator++() noexcept -> const_iterator & {
    node_ = node_->next_;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> const_iterator {
    const_iterator const copy(node_);
    operator++();
    return copy;
  }

 private:
  node const * node_;
};

template <typename T>
class sealable_stack<T>::node final {
 private:
  static constexpr auto kSeal(node const * next) noexcept -> node {
    return node(next);
  };

 private:
  explicit constexpr node(node const * next) noexcept
      : value_(std::nullopt), next_(next) {}

 public:
  template <typename... Args>
  explicit constexpr node(Args &&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>)
      : value_(std::in_place, std::forward<Args>(args)...), next_(nullptr) {}

 private:
  [[nodiscard]] constexpr auto is_seal() const noexcept -> bool {
    return !value_.has_value();
  }

 public:
  constexpr auto value() const noexcept -> T const & { return *value_; }

 private:
  std::optional<T> value_;
  mutable node const * next_;

 private:
  /**
   * @note For accessing the pointer to the next node
   */
  friend class sealable_stack<T>;
};

}  // namespace swiss
