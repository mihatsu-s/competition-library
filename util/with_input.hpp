#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include "meta.hpp"

namespace mihatsu {

template <typename T, typename = void>
struct with_input {
    static_assert(_internal::false_v<T>, "with_input does not support this type");
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
        : val(std::forward<T>(val)) {}
    constexpr inline operator const T&() const noexcept {
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
        : T{std::forward<T>(val)} {}
};

}  // namespace mihatsu