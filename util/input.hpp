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
    inline constexpr arithmetic_with_input(const T& val) noexcept
        : val(val) {}
    inline constexpr arithmetic_with_input(T&& val) noexcept
        : val(val) {}
    inline constexpr operator T() const noexcept {
        return val;
    }
    inline constexpr operator T&() noexcept {
        return val;
    }
};

template <typename T, std::enable_if_t<std::is_class_v<T>>* = nullptr>
struct class_with_input : public T {
    inline class_with_input() {
        std::cin >> *this;
    }
    inline constexpr class_with_input(const T& val) noexcept
        : T{val} {}
    inline constexpr class_with_input(T&& val) noexcept
        : T{val} {}
};

}  // namespace mihatsu

#endif
