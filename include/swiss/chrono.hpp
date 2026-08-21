#pragma once

#include <atomic>
#include <chrono>

namespace swiss {

/**
 * @brief Clock that does not tick by itself and is manually manipulated by the
 * user code
 * @note Thread-safe
 */
template <typename Base = std::chrono::system_clock>
class mocked_clock final {
 public:
  using duration   = Base::duration;
  using rep        = duration::rep;
  using period     = duration::period;
  using time_point = std::chrono::time_point<mocked_clock>;

 public:
  static constexpr auto now() noexcept -> time_point {
    return time_point{duration{now_.load(std::memory_order::relaxed)}};
  }

  static constexpr auto shift(duration duration) noexcept {
    now_.fetch_add(duration.count(), std::memory_order::relaxed);
  }

  static constexpr auto set(time_point time_point) noexcept {
    now_.store(time_point.time_since_epoch().count(),
               std::memory_order::relaxed);
  }

 private:
  static inline std::atomic<rep> now_;
};

}  // namespace swiss
