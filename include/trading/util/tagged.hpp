#ifndef TAGGED_HPP
#define TAGGED_HPP

#include <compare>
#include <functional>

TRADING_NAMESPACE_BEGIN

namespace util {

template <typename Value, typename Tag>
class Tagged {
 public:
  using ValueType = Value;
  using TagType = Tag;

  Tagged() = default;

  explicit Tagged(const ValueType& value)
      : value_(value) {
  }

  explicit Tagged(ValueType&& value)
      : value_(std::move(value)) {
  }

  const ValueType& operator*() const noexcept {
    return value_;
  }

  ValueType operator*() noexcept {
    return value_;
  }

  friend auto operator<=>(const Tagged&, const Tagged&) = default;

 protected:
  ValueType value_{};
};

template <typename TaggedValue>
struct TaggedHasher {
  std::size_t operator()(const TaggedValue& value) const {
    return std::hash<typename TaggedValue::ValueType>{}(*value);
  }
};

}  // namespace util

TRADING_NAMESPACE_END

#endif