#pragma once

#include <functional>
#include <type_traits>

namespace mihatsu {

template <typename T, typename U>
inline T& chmax(T& x, U&& y) {
    return (x < y) ? (x = std::forward<U>(y)) : x;
}
template <typename T, typename U>
inline T& chmin(T& x, U&& y) {
    return (y < x) ? (x = std::forward<U>(y)) : x;
}

template <class T, class U, std::enable_if_t<!std::is_same_v<T, U>>* = nullptr>
inline constexpr std::common_type_t<T, U> max(const T& t, const U& u) {
    return t < u ? static_cast<std::common_type_t<T, U>>(u) : static_cast<std::common_type_t<T, U>>(t);
}
template <class T, class U, std::enable_if_t<!std::is_same_v<T, U>>* = nullptr>
inline constexpr std::common_type_t<T, U> min(const T& t, const U& u) {
    return t < u ? static_cast<std::common_type_t<T, U>>(t) : static_cast<std::common_type_t<T, U>>(u);
}

}  // namespace mihatsu
