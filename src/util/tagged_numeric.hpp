#ifndef TAGGED_NUMERIC_HPP
#define TAGGED_NUMERIC_HPP

#include "tagged.hpp"

namespace util {

template <typename Numeric, typename Tag>
class TaggedNumeric : public Tagged<Numeric, Tag> {
 public:
  using BaseType = Tagged<Numeric, Tag>;
  using BaseType::Tagged;

 public:
  TaggedNumeric& operator+=(const TaggedNumeric& other) & noexcept {
    this->value_ += other.value_;
    return *this;
  }

  TaggedNumeric& operator-=(const TaggedNumeric& other) & noexcept {
    this->value_ -= other.value_;
    return *this;
  }

  TaggedNumeric operator+(const TaggedNumeric& other) const& noexcept {
    TaggedNumeric result(*this);
    result += other;
    return result;
  }

  TaggedNumeric operator-(const TaggedNumeric& other) const& noexcept {
    TaggedNumeric result(*this);
    result -= other;
    return result;
  }
};

}  // namespace util

#endif