#ifndef TAGGED_NUMERIC_HPP
#define TAGGED_NUMERIC_HPP

#include <cstdint>

#include "tagged.hpp"

namespace util {

template <typename Numeric, typename Tag>
class TaggedNumeric : public Tagged<Numeric, Tag> {
 public:
  using BaseType = Tagged<Numeric, Tag>;
  using BaseType::Tagged;

  TaggedNumeric& operator+=(const TaggedNumeric& other) & noexcept {
    this->value_ += other.value_;
    return *this;
  }

  TaggedNumeric& operator-=(const TaggedNumeric& other) & noexcept {
    this->value_ -= other.value_;
    return *this;
  }

  TaggedNumeric& operator*=(std::int64_t num) & noexcept {
    this->value_ *= num;
    return *this;
  }

  TaggedNumeric& operator/=(std::int64_t num) & noexcept {
    this->value_ /= num;
    return *this;
  }

  const TaggedNumeric operator+() const noexcept {
    return TaggedNumeric(this->value_);
  }

  const TaggedNumeric operator-() const noexcept {
    return TaggedNumeric(this->value_ * -1);
  }
};

template <typename Numeric, typename Tag>
const TaggedNumeric<Numeric, Tag> operator+(
    const TaggedNumeric<Numeric, Tag>& first,
    const TaggedNumeric<Numeric, Tag>& second) {
  TaggedNumeric<Numeric, Tag> result(first);
  result += second;
  return result;
}

template <typename Numeric, typename Tag>
const TaggedNumeric<Numeric, Tag> operator-(
    const TaggedNumeric<Numeric, Tag>& first,
    const TaggedNumeric<Numeric, Tag>& second) {
  TaggedNumeric<Numeric, Tag> result(first);
  result -= second;
  return result;
}

template <typename Numeric, typename Tag>
const TaggedNumeric<Numeric, Tag> operator*(
    const TaggedNumeric<Numeric, Tag>& tagged_num, std::int64_t num) {
  TaggedNumeric<Numeric, Tag> result(tagged_num);
  result *= num;
  return result;
}

template <typename Numeric, typename Tag>
const TaggedNumeric<Numeric, Tag> operator*(
    std::int64_t num, const TaggedNumeric<Numeric, Tag>& tagged_num) {
  return tagged_num * num;
}

template <typename Numeric, typename Tag>
const TaggedNumeric<Numeric, Tag> operator/(
    const TaggedNumeric<Numeric, Tag>& tagged_num, std::int64_t num) {
  TaggedNumeric<Numeric, Tag> result(tagged_num);
  result /= num;
  return result;
}

template <typename Numeric, typename Tag>
const TaggedNumeric<Numeric, Tag> operator/(
    std::int64_t num, const TaggedNumeric<Numeric, Tag>& tagged_num) {
  return tagged_num / num;
}

}  // namespace util

#endif