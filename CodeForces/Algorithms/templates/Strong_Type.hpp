#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Strong Nominal Typing Utility */

namespace cp {

template <class T, class Tag>
class StrongType {
public:
  using value_type = T;
  using tag_type = Tag;

  constexpr StrongType()
    requires std::default_initializable<T>
  = default;

  explicit constexpr StrongType(const T& value) : value_(value) {}
  explicit constexpr StrongType(T&& value) : value_(std::move(value)) {}

  [[nodiscard]] constexpr const T& get() const& noexcept { return value_; }
  [[nodiscard]] constexpr T& get() & noexcept { return value_; }
  [[nodiscard]] constexpr T&& get() && noexcept { return std::move(value_); }

  [[nodiscard]] explicit constexpr operator const T&() const noexcept { return value_; }
  [[nodiscard]] explicit constexpr operator T&() noexcept { return value_; }

  friend constexpr bool operator==(const StrongType&, const StrongType&) = default;
  friend constexpr auto operator<=>(const StrongType&, const StrongType&)
    requires std::three_way_comparable<T>
  = default;

  constexpr StrongType& operator+=(const StrongType& other)
    requires requires(T a, const T& b) { a += b; }
  {
    value_ += other.value_;
    return *this;
  }

  constexpr StrongType& operator-=(const StrongType& other)
    requires requires(T a, const T& b) { a -= b; }
  {
    value_ -= other.value_;
    return *this;
  }

private:
  T value_{};
};

template <class T, class Tag>
[[nodiscard]] constexpr const T& unwrap(const StrongType<T, Tag>& value) noexcept {
  return value.get();
}

template <class T, class Tag>
[[nodiscard]] constexpr StrongType<T, Tag> make_strong(T value) {
  return StrongType<T, Tag>(std::move(value));
}

} // namespace cp

namespace std {

template <class T, class Tag>
struct hash<cp::StrongType<T, Tag>> {
  size_t operator()(const cp::StrongType<T, Tag>& value) const
      noexcept(noexcept(std::hash<T>{}(value.get()))) {
    return std::hash<T>{}(value.get());
  }
};

} // namespace std
