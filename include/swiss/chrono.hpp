#pragma once

#include <chrono>

namespace swiss {

/**
 * @brief Clock that does not tick by itself and is manually manipulated by the
 * user code
 */
template <typename Base = std::chrono::system_clock>
class mocked_clock final {
 public:
  using duration   = Base::duration;
  using rep        = duration::rep;
  using period     = duration::period;
  using time_point = std::chrono::time_point<Base>;

 public:
  static constexpr auto now() noexcept -> time_point { return now_; }
  static constexpr auto shift(duration duration) noexcept { now_ += duration; }
  static constexpr auto set(time_point time_point) noexcept {
    now_ = time_point;
  }

 private:
  static inline time_point now_ = Base::now();
};

}  // namespace swiss
