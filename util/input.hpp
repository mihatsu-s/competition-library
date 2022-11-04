#ifndef MIHATSU_UTIL_INPUT_HPP
#define MIHATSU_UTIL_INPUT_HPP

#include <array>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace mihatsu {

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
struct arithmetic_with_input {
   private:
    T val;

   public:
    inline arithmetic_with_input() {
        std::cin >> val;
    }
    constexpr inline arithmetic_with_input(const T& val) noexcept
        : val(val) {}
    constexpr inline arithmetic_with_input(T&& val) noexcept
        : val(val) {}
    constexpr inline operator T() const noexcept {
        return val;
    }
    constexpr inline operator T&() noexcept {
        return val;
    }
};

template <typename T, std::enable_if_t<std::is_class_v<T>>* = nullptr>
struct class_with_input : public T {
    inline class_with_input() {
        std::cin >> *this;
    }
    constexpr inline class_with_input(const T& val) noexcept
        : T{val} {}
    constexpr inline class_with_input(T&& val) noexcept
        : T{val} {}
};

}  // namespace mihatsu

#endif
