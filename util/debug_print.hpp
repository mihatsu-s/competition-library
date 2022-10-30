#ifndef MIHATSU_UTIL_DEBUG_PRINT_HPP
#define MIHATSU_UTIL_DEBUG_PRINT_HPP 1

#ifdef DEBUG

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include "type_traits.hpp"

template <typename _CharT, typename _Traits, typename T>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>&, const T&);

namespace mihatsu {
namespace _internal {

struct DebugPrinter {
    std::ostream& os = std::cerr;
    const std::string sep = "  ";

    const char* str;
    DebugPrinter(const char* file, int line, const char* str)
        : str(str) {
        os << file << ":" << line << sep;
    }

    void operator()() {
        os << std::endl;
    }
    template <typename Head, typename... Tail>
    void operator()(const Head& head, const Tail&... tail) {
        int paren_level = 0;
        for (; *str != 0; ++str) {
            if (*str == '(') {
                ++paren_level;
            } else if (*str == ')') {
                --paren_level;
            } else if (paren_level == 0 && *str == ',') {
                break;
            }
            if (*str != ' ') std::cerr << *str;
        }
        if (*str != 0) ++str;
        os << "=";
        print_value(head);
        os << sep;
        this->operator()(tail...);
    }

    template <typename T>
    inline void print_value(const T& x) {
        if constexpr (is_maplike_v<T>) {
            os << "{";
            bool is_first = true;
            for (const auto& [key, value] : x) {
                os << (is_first ? "" : " ");
                is_first = false;
                print_value(key);
                os << ":";
                print_value(value);
            }
            os << "}";
        } else if constexpr (is_container_v<T>) {
            os << "[";
            bool is_first = true;
            for (const auto& value : x) {
                os << (is_first ? "" : " ");
                is_first = false;
                print_value(value);
            }
            os << "]";
        } else if constexpr (is_atcoder_modint_v<T>) {
            os << x.val();
        } else {
            os << x;
        }
    }
    inline void print_value(const std::string& str) {
        os << str;
    }
    inline void print_value(const char* str) {
        os << str;
    }
    template <typename T1, typename T2>
    inline void print_value(const std::pair<T1, T2>& pair) {
        print_value(std::tuple<T1, T2>(pair.first, pair.second));
    }
    template <typename... T>
    inline void print_value(const std::tuple<T...>& tuple) {
        print_tuple_impl<0>(tuple);
    }
    template <int index, typename... T>
    void print_tuple_impl(const std::tuple<T...>& tuple) {
        if constexpr (index == 0) os << "(";
        if constexpr (index < sizeof...(T)) {
            if constexpr (index > 0) os << ",";
            print_value(std::get<index>(tuple));
            print_tuple_impl<index + 1>(tuple);
        } else {
            os << ")";
        }
    }
};

}  // namespace _internal
}  // namespace mihatsu

#define debug_print(...) (mihatsu::_internal::DebugPrinter(__FILE__, __LINE__, #__VA_ARGS__)(__VA_ARGS__))

#else
#define debug_print(...)
#endif

#endif
