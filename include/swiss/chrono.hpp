#pragma once

#include <atomic>
#include <chrono>

namespace swiss {

/**
 * @brief Clock that does not tick by itself and is manually manipulated by the
 * user code. Shared across all code
 * @note Thread-safe
 */
template <typename Base = std::chrono::system_clock>
class mocked_clock final {
 public:
  using duration                  = typename Base::duration;
  using rep                       = typename duration::rep;
  using period                    = typename duration::period;
  using time_point                = std::chrono::time_point<mocked_clock>;
  static bool constexpr is_steady = Base::is_steady;

 public:
  static constexpr auto now() noexcept -> time_point {
    return time_point{duration{now_.load(std::memory_order::relaxed)}};
  }

  /**
   * @brief Add duration to the clock
   */
  static constexpr void shift(duration duration) noexcept {
    now_.fetch_add(duration.count(), std::memory_order::relaxed);
  }

  /**
   * @brief Manually set clocks value
   */
  static constexpr void set(time_point time_point) noexcept {
    now_.store(time_point.time_since_epoch().count(),
               std::memory_order::relaxed);
  }

  /**
   * @brief Set mocked clock to the now() value of the base clock
   */
  static void sync() noexcept { set(Base::now()); }

 private:
  static inline std::atomic<rep> now_ = Base::now().time_since_epoch().count();
};

/**
 * @brief Clock that does not tick by itself and is manually manipulated by the
 * user code. Shared across all code
 * @note Clock is stored on per-thread basis
 */
template <typename Base = std::chrono::system_clock>
class tls_mocked_clock final {
 public:
  using duration                  = typename Base::duration;
  using rep                       = typename duration::rep;
  using period                    = typename duration::period;
  using time_point                = std::chrono::time_point<tls_mocked_clock>;
  static bool constexpr is_steady = Base::is_steady;

 public:
  static constexpr auto now() noexcept -> time_point { return now_; }

  /**
   * @brief Add duration to the clock
   */
  static constexpr void shift(duration duration) noexcept { now_ += duration; }

  /**
   * @brief Manually set clocks value
   */
  static constexpr void set(time_point time_point) noexcept {
    now_ = time_point;
  }

  /**
   * @brief Set mocked clock to the now() value of the base clock
   */
  static void sync() noexcept { set(Base::now()); }

 private:
  static thread_local inline time_point now_{Base::now().time_since_epoch()};
};

}  // namespace swiss
