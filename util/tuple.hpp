#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <tuple>
#include <utility>
#include "type_traits.hpp"

namespace mihatsu {
namespace _internal {

// TODO: implement pluseq, minuseq, multiplieseq, divideseq

}  // namespace _internal
}  // namespace mihatsu

namespace std {

template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T> && mihatsu::_internal::is_pairlike_v<U>>* = nullptr>
constexpr inline auto operator+(const T& lhs, const U& rhs) {
    return pair(lhs.first + rhs.first, lhs.second + rhs.second);
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T> && mihatsu::_internal::is_pairlike_v<U>>* = nullptr>
constexpr inline auto operator-(const T& lhs, const U& rhs) {
    return pair(lhs.first - rhs.first, lhs.second - rhs.second);
}
template <typename T, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
constexpr inline auto operator-(const T& pair) {
    return pair(-pair.first, -pair.second);
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
constexpr inline auto operator*(const T& lhs, const U& rhs) {
    return pair(lhs.first * rhs, lhs.second * rhs);
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<U>>* = nullptr>
constexpr inline auto operator*(const T& lhs, const U& rhs) {
    return pair(lhs * rhs.first, lhs * rhs.second);
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
constexpr inline auto operator/(const T& lhs, const U& rhs) {
    return pair(lhs.first / rhs, lhs.second / rhs);
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T> && mihatsu::_internal::is_pairlike_v<U>>* = nullptr>
constexpr inline auto operator+=(T& lhs, const U& rhs) {
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T> && mihatsu::_internal::is_pairlike_v<U>>* = nullptr>
constexpr inline auto operator-=(T& lhs, const U& rhs) {
    lhs.first -= rhs.first;
    lhs.second -= rhs.second;
    return lhs;
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
constexpr inline auto operator*=(T& lhs, const U& rhs) {
    lhs.first *= rhs;
    lhs.second *= rhs;
    return lhs;
}
template <typename T, typename U, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
constexpr inline auto operator/=(T& lhs, const U& rhs) {
    lhs.first /= rhs;
    lhs.second /= rhs;
    return lhs;
}

// TODO: overload operators for tuple

}  // namespace std

namespace mihatsu {
namespace _internal {

inline std::size_t hash_sequence() {
    return 0;
}
template <typename Head, typename... Tail>
inline std::size_t hash_sequence(const Head& head, const Tail&... tail) {
    const auto seed = hash_sequence(tail...);
    return std::hash<Head>()(head) + 0x4cc1a90e + (seed << 23) + (seed >> 11) ^ seed;
}

}  // namespace _internal
}  // namespace mihatsu

namespace std {

template <typename T1, typename T2>
class hash<pair<T1, T2>> {
   public:
    inline size_t operator()(const pair<T1, T2>& pair) const {
        return mihatsu::_internal::hash_sequence(pair.first, pair.second);
    }
};

template <typename... T>
class hash<tuple<T...>> {
   public:
    inline size_t operator()(const tuple<T...>& tuple) const {
        return apply(mihatsu::_internal::hash_sequence<T...>, tuple);
    }
};

}  // namespace std
