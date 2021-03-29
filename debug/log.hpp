#include <iostream>

#define debug_log(...) __debug_log(#__VA_ARGS__, __VA_ARGS__)

void __debug_log(const char* str) {
    std::clog << std::endl;
}

template <typename Head, typename... Tail>
void __debug_log(const char* str, Head&& head, Tail&&... tail) {
#ifndef DEBUG
    return;
#endif
    int paren_level = 0;
    for (; *str != 0; ++str) {
        if (*str == '(') {
            ++paren_level;
        } else if (*str == ')') {
            --paren_level;
        } else if (paren_level == 0 && *str == ',') {
            break;
        }
        std::clog << *str;
    }
    if (*str != 0) ++str;
    std::clog << " = " << head << " \t";
    __debug_log(str, std::forward<Tail>(tail)...);
}
