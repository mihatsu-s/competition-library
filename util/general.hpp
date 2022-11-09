#pragma once

#include <functional>

namespace mihatsu {

template <typename T, typename U>
inline T& chmax(T& x, U&& y) {
    return (x < y) ? (x = std::forward<U>(y)) : x;
}
template <typename T, typename U>
inline T& chmin(T& x, U&& y) {
    return (y < x) ? (x = std::forward<U>(y)) : x;
}

}  // namespace mihatsu
