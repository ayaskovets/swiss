#pragma once

#include <iostream>
#include <string>

namespace swiss {

/**
 * @brief A boilerplate class that each C++ developer has written at least once.
 * Used for logging special member function calls to std::ostream
 * @note Output stream reference is attached to the object and is immutable
 */
class rule_of_five_logger {
 private:
  static constexpr auto const kMovedFromTag = "<moved>";

 public:
  explicit constexpr rule_of_five_logger(std::ostream & ostream,
                                         std::string tag) noexcept
      : ostream_(ostream), tag_(std::move(tag)) {
    ostream_ << "rule_of_five_logger::rule_of_five_logger({" << tag_ << "})\n";
  }

  constexpr rule_of_five_logger(rule_of_five_logger const & that) noexcept
      : ostream_(that.ostream_), tag_(that.tag_) {
    ostream_ << "rule_of_five_logger::rule_of_five_logger(rule_of_five_logger "
                "const&{"
             << that.tag_ << "})\n";
  }

  constexpr rule_of_five_logger(rule_of_five_logger && that) noexcept
      : ostream_(that.ostream_), tag_(std::move(that.tag_)) {
    ostream_
        << "rule_of_five_logger::rule_of_five_logger(rule_of_five_logger &&{"
        << that.tag_ << "})\n";
    that.tag_ = kMovedFromTag;
  }

  constexpr auto operator=(rule_of_five_logger const & that) noexcept
      -> rule_of_five_logger & {
    if (this != &that) {
      tag_ = that.tag_;
    }
    ostream_ << "rule_of_five_logger::operator=(rule_of_five_logger const&{"
             << that.tag_ << "})\n";
    return *this;
  }

  constexpr auto operator=(rule_of_five_logger && that) noexcept
      -> rule_of_five_logger & {
    if (this != &that) {
      tag_ = that.tag_;
    }
    ostream_ << "rule_of_five_logger::operator=(rule_of_five_logger &&{"
             << that.tag_ << "})\n";
    return *this;
  }

  constexpr ~rule_of_five_logger() noexcept {
    ostream_ << "rule_of_five_logger::~rule_of_five_logger({" << tag_ << "})\n";
  }

 private:
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  std::ostream & ostream_;
  std::string tag_;
};

}  // namespace swiss
