#pragma once

#include <array>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include "type_traits.hpp"

namespace mihatsu {

template <typename T, typename = void>
struct with_input {
    static_assert(_internal::false_v<T>, "mihatsu::with_input does not support this type");
};

template <typename T>
struct with_input<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
   private:
    T val;

   public:
    inline with_input() {
        std::cin >> val;
    }
    constexpr inline with_input(const T& val) noexcept
        : val(val) {}
    constexpr inline with_input(T&& val) noexcept
        : val(val) {}
    constexpr inline operator T() const noexcept {
        return val;
    }
    constexpr inline operator T&() noexcept {
        return val;
    }
};

template <typename T>
struct with_input<T, std::enable_if_t<std::is_class_v<T>>> : public T {
    using T::T;
    inline with_input() {
        std::cin >> *this;
    }
    constexpr inline with_input(const T& val) noexcept
        : T{val} {}
    constexpr inline with_input(T&& val) noexcept
        : T{val} {}
};

namespace _internal {

template <int index, typename... T>
inline std::istream& input_tuple_impl(std::istream& is, std::tuple<T...>& tuple) {
    if constexpr (index < sizeof...(T)) {
        is >> std::get<index>(tuple);
        return input_tuple_impl<index + 1>(is, tuple);
    } else {
        return is;
    }
}

}  // namespace _internal

}  // namespace mihatsu

namespace std {

template <typename T, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
inline istream& operator>>(istream& is, T& pair) {
    return is >> pair.first >> pair.second;
}

template <typename... T>
inline istream& operator>>(istream& is, tuple<T...>& tuple) {
    return mihatsu::_internal::input_tuple_impl<0>(is, tuple);
}

template <class T, class Allocator>
inline istream& operator>>(istream& is, vector<T, Allocator>& vector) {
    for (auto&& p : vector) cin >> p;
    return is;
}

}  // namespace std
