#pragma once

#include <iostream>
#include <string>

namespace swiss {

class RuleOfFiveLogger {
 private:
  static constexpr auto const kMovedFromTag = "<moved>";

 public:
  explicit constexpr RuleOfFiveLogger(std::ostream & ostream,
                                      std::string tag) noexcept
      : ostream_(ostream), tag_(std::move(tag)) {
    ostream_ << "RuleOfFiveLogger::RuleOfFiveLogger({" << tag_ << "})\n";
  }

  constexpr RuleOfFiveLogger(RuleOfFiveLogger const & that) noexcept
      : ostream_(that.ostream_), tag_(that.tag_) {
    ostream_ << "RuleOfFiveLogger::RuleOfFiveLogger(RuleOfFiveLogger const&{"
             << that.tag_ << "})\n";
  }

  constexpr RuleOfFiveLogger(RuleOfFiveLogger && that) noexcept
      : ostream_(that.ostream_), tag_(std::move(that.tag_)) {
    ostream_ << "RuleOfFiveLogger::RuleOfFiveLogger(RuleOfFiveLogger &&{"
             << that.tag_ << "})\n";
    that.tag_ = kMovedFromTag;
  }

  constexpr auto operator=(RuleOfFiveLogger const & that) noexcept
      -> RuleOfFiveLogger & {
    if (this != &that) {
      new (this) RuleOfFiveLogger(that);
    }
    ostream_ << "RuleOfFiveLogger::operator=(RuleOfFiveLogger const&{"
             << that.tag_ << "})\n";
    return *this;
  }

  constexpr auto operator=(RuleOfFiveLogger && that) noexcept
      -> RuleOfFiveLogger & {
    if (this != &that) {
      new (this) RuleOfFiveLogger(std::move(that));
    }
    ostream_ << "RuleOfFiveLogger::operator=(RuleOfFiveLogger &&{" << that.tag_
             << "})\n";
    return *this;
  }

  constexpr ~RuleOfFiveLogger() noexcept {
    ostream_ << "RuleOfFiveLogger::~RuleOfFiveLogger({" << tag_ << "})\n";
  }

 private:
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  std::ostream & ostream_;
  std::string tag_;
};

}  // namespace swiss
