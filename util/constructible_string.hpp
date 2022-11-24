#pragma once

#include <sstream>
#include <string>

namespace mihatsu {

template <class charT, class traits = std::char_traits<charT>, class Allocator = std::allocator<charT>>
class constructible_string : public std::basic_string<charT, traits, Allocator> {
    template <class T>
    inline static std::basic_string<charT, traits, Allocator> init(const T& val) {
        std::basic_ostringstream<charT, traits, Allocator> ss;
        ss << val;
        return ss.str();
    }

   public:
    template <class T>
    inline constructible_string(const T& val)
        : std::basic_string<charT, traits, Allocator>{init(val)} {}
};

}  // namespace mihatsu
