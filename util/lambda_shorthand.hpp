#pragma once

#include <tuple>
#include "../internal/meta.hpp"

namespace mihatsu::_internal::lambda_shorthand {
template <std::size_t N>
struct _MissingArg {};
}  // namespace mihatsu::_internal::lambda_shorthand

#define L(...) ([&](auto&&... _args) -> auto&& {                                                                                                                                                          \
    auto _expr = [&](auto&& $1, auto&& $2, auto&& $3, auto&& $4, auto&& $5, auto&& $6, auto&& $7, auto&& $8, auto&& $9) -> auto&& { return (__VA_ARGS__); };                                              \
    auto _argt = std::forward_as_tuple(_args...);                                                                                                                                                         \
    using mihatsu::_internal::lambda_shorthand::_MissingArg;                                                                                                                                              \
                                                                                                                                                                                                          \
    if constexpr (sizeof...(_args) == 0) {                                                                                                                                                                \
        return _expr(_MissingArg<1>(), _MissingArg<2>(), _MissingArg<3>(), _MissingArg<4>(), _MissingArg<5>(), _MissingArg<6>(), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());                   \
    } else if constexpr (sizeof...(_args) == 1) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), _MissingArg<2>(), _MissingArg<3>(), _MissingArg<4>(), _MissingArg<5>(), _MissingArg<6>(), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());                 \
    } else if constexpr (sizeof...(_args) == 2) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), _MissingArg<3>(), _MissingArg<4>(), _MissingArg<5>(), _MissingArg<6>(), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());               \
    } else if constexpr (sizeof...(_args) == 3) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), _MissingArg<4>(), _MissingArg<5>(), _MissingArg<6>(), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());             \
    } else if constexpr (sizeof...(_args) == 4) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), std::get<3>(_argt), _MissingArg<5>(), _MissingArg<6>(), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());           \
    } else if constexpr (sizeof...(_args) == 5) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), std::get<3>(_argt), std::get<4>(_argt), _MissingArg<6>(), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());         \
    } else if constexpr (sizeof...(_args) == 6) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), std::get<3>(_argt), std::get<4>(_argt), std::get<5>(_argt), _MissingArg<7>(), _MissingArg<8>(), _MissingArg<9>());       \
    } else if constexpr (sizeof...(_args) == 7) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), std::get<3>(_argt), std::get<4>(_argt), std::get<5>(_argt), std::get<6>(_argt), _MissingArg<8>(), _MissingArg<9>());     \
    } else if constexpr (sizeof...(_args) == 8) {                                                                                                                                                         \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), std::get<3>(_argt), std::get<4>(_argt), std::get<5>(_argt), std::get<6>(_argt), std::get<7>(_argt), _MissingArg<9>());   \
    } else {                                                                                                                                                                                              \
        return _expr(std::get<0>(_argt), std::get<1>(_argt), std::get<2>(_argt), std::get<3>(_argt), std::get<4>(_argt), std::get<5>(_argt), std::get<6>(_argt), std::get<7>(_argt), std::get<8>(_argt)); \
    }                                                                                                                                                                                                     \
})
