#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include "../internal/meta.hpp"

namespace mihatsu {

template <typename T, typename = void>
struct with_input {
    static_assert(_internal::false_v<T>, "with_input does not support this type");
};

template <typename T>
struct with_input<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
   private:
    T val;

    class input_error : public std::exception {
        const char* what() const noexcept {
            return "failed reading std::cin";
        }
    };

   public:
    inline with_input() {
        std::cin >> val;
        if (!std::cin) throw input_error();
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
   private:
    class input_error : public std::exception {
        const char* what() const noexcept {
            return "failed reading std::cin";
        }
    };

   public:
    using T::T;
    inline with_input() {
        std::cin >> *this;
        if (!std::cin) throw input_error();
    }
    constexpr inline with_input(const T& val) noexcept
        : T{val} {}
    constexpr inline with_input(T&& val) noexcept
        : T{std::forward<T>(val)} {}
};

}  // namespace mihatsu

namespace std {

template <typename T, typename U>
struct common_type<mihatsu::with_input<T>, U> {
    using type = typename common_type<T, U>::type;
};
template <typename T, typename U>
struct common_type<T, mihatsu::with_input<U>> {
    using type = typename common_type<T, U>::type;
};
template <typename T, typename U>
struct common_type<mihatsu::with_input<T>, mihatsu::with_input<U>> {
    using type = typename common_type<T, U>::type;
};

}  // namespace std
