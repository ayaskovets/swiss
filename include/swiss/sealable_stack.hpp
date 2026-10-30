#pragma once

#include <atomic>
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
  class iterator;
  class const_iterator;
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
  [[nodiscard]] constexpr auto try_push(node & node) noexcept -> bool {
    class node * expected = top_.load(std::memory_order::acquire);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-do-while)
    do {
      if (node::is_seal(expected)) {
        return false;
      }

      node.next_ = expected;
    } while (!top_.compare_exchange_weak(expected, &node,
                                         std::memory_order::acq_rel,
                                         std::memory_order::acquire));
    return true;
  }

  constexpr void seal() noexcept {
    node * expected = top_.load(std::memory_order::relaxed);
    while (!top_.compare_exchange_weak(expected, node::seal(expected),
                                       std::memory_order::release,
                                       std::memory_order::relaxed)) {
    }
  }

 public:
  constexpr auto top() const noexcept -> node const * {
    return node::unseal(top_.load(std::memory_order::acquire));
  }

  constexpr auto top() noexcept -> node * {
    return node::unseal(top_.load(std::memory_order::acquire));
  }

  [[nodiscard]] constexpr auto empty() const noexcept -> bool { return !top(); }

  [[nodiscard]] constexpr auto sealed() const noexcept -> bool {
    return node::is_seal(top_.load(std::memory_order::acquire));
  }

 private:
  std::atomic<node *> top_;
};

template <typename T>
class sealable_stack<T>::iterator final {
 public:
  using value_type      = T;
  using reference       = T &;
  using pointer         = T *;
  using const_pointer   = T const *;
  using const_reference = T const &;
  using difference_type = std::ptrdiff_t;

 public:
  constexpr iterator() noexcept : node_(nullptr) {}
  explicit constexpr iterator(node * node) noexcept : node_(node) {}

 public:
  constexpr auto operator==(iterator const & that) const noexcept
      -> bool = default;

  constexpr auto operator*() noexcept -> reference { return node_->value(); }
  constexpr auto operator*() const noexcept -> reference {
    return node_->value();
  }

  constexpr auto operator++() noexcept -> iterator & {
    node_ = node_->next_;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> iterator {
    iterator const copy(node_);
    operator++();
    return copy;
  }

 private:
  node * node_;
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
  static constexpr std::uintptr_t kSealedBit = 1;

  static constexpr auto seal(node * next) noexcept -> node * {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<node *>(reinterpret_cast<std::uintptr_t>(next) |
                                    kSealedBit);
  }

  static constexpr auto unseal(node * next) noexcept -> node * {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto const address = reinterpret_cast<std::uintptr_t>(next) & ~kSealedBit;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return address == 0 ? nullptr : reinterpret_cast<node *>(address);
  }

  [[nodiscard]] static constexpr auto is_seal(node const * ptr) noexcept
      -> bool {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return (reinterpret_cast<std::uintptr_t>(ptr) & kSealedBit) != 0;
  }

 public:
  template <typename... Args>
  explicit constexpr node(Args &&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>)
      : value_(std::forward<Args>(args)...), next_(nullptr) {}

 public:
  constexpr auto value() const noexcept -> T const & { return value_; }
  constexpr auto value() noexcept -> T & { return value_; }

 private:
  T value_;
  node * next_;

 private:
  /**
   * @note For accessing the pointer to the next node
   */
  friend class sealable_stack<T>;
};

}  // namespace swiss
