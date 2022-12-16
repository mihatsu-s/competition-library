#pragma once

#include <iterator>
#include <type_traits>
#include "../internal/macro.h"
#include "../internal/meta.hpp"

namespace mihatsu::_internal::rep_impl {

template <class T>
struct numeric_range {
    T size;
    constexpr inline numeric_range(const T& size) noexcept
        : size(size) {}

    class iterator {
        friend numeric_range<T>;

        T val;
        constexpr inline iterator(const T& val) noexcept
            : val(val) {}

       public:
        inline iterator& operator++() noexcept {
            ++val;
            return *this;
        }
        constexpr inline T& operator*() noexcept {
            return val;
        }
        constexpr inline bool operator!=(const iterator& other) const noexcept {
            return val < other.val;
        }
    };
    constexpr inline iterator begin() const noexcept { return iterator(0); }
    constexpr inline iterator end() const noexcept { return iterator(size); }

    class reverse_iterator {
        friend numeric_range<T>;

        T val;
        constexpr inline reverse_iterator(const T& val) noexcept
            : val(val) {}

       public:
        inline reverse_iterator& operator++() noexcept {
            --val;
            return *this;
        }
        constexpr inline T& operator*() noexcept {
            return val;
        }
        constexpr inline bool operator!=(const reverse_iterator& other) const noexcept {
            return val >= other.val;
        }
    };
    constexpr inline reverse_iterator rbegin() const noexcept { return reverse_iterator(size - 1); }
    constexpr inline reverse_iterator rend() const noexcept { return reverse_iterator(0); }
};

template <class T, std::enable_if_t<is_container_v<std::remove_reference_t<T>>>* = nullptr>
constexpr inline decltype(auto) range(T&& x) {
    return x;
}
template <class T, std::enable_if_t<!is_container_v<std::remove_reference_t<T>>>* = nullptr>
constexpr inline decltype(auto) range(T&& x) {
    return numeric_range<std::remove_reference_t<T>>(x);
}

template <std::nullptr_t = nullptr>
struct breaking_state {
    static bool flag;
    static inline void do_break() noexcept {
        flag = true;
    }
    static inline bool check() noexcept {
        bool res = !flag;
        flag = false;
        return res;
    }
};
template <std::nullptr_t _>
inline bool breaking_state<_>::flag = false;

}  // namespace mihatsu::_internal::rep_impl

#define rep(var, ...)                                                                        \
    if (mihatsu::_internal::rep_impl::breaking_state<>::check())                             \
        MIHATSU_CONDITIONAL(MIHATSU_IS_BRACKETED(var), _MIHATSU_REP_BRACKETED, _MIHATSU_REP) \
    (var, __VA_ARGS__)
#define _MIHATSU_REP(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP, __VA_ARGS__)
#define _MIHATSU_REP2(i, n) \
    if (auto&& _##i##_range = (n); true) _MIHATSU_REP_RANGE(i, mihatsu::_internal::rep_impl::range(_##i##_range))
#define _MIHATSU_REP3(i, a, b) for (std::common_type_t<decltype(a), decltype(b)> i = (a), _##i##_end = (b); i != _##i##_end; ++i)
#define _MIHATSU_REP4(i, a, b, c) for (std::common_type_t<decltype(a), decltype(b), decltype(c)> i = (a), _##i##_end = (b), _##i##_step = (c); _##i##_step > 0 ? i < _##i##_end : i > _##i##_end; i += _##i##_step)
#define _MIHATSU_REP_BRACKETED(vars, ...) _MIHATSU_REP_RANGE(_MIHATSU_REP_BRACKETED_VARS vars, (__VA_ARGS__))
#define _MIHATSU_REP_BRACKETED_VARS(...) [__VA_ARGS__]
#define _MIHATSU_REP_RANGE(var, range) for (auto&& var : range)

#define rep_rev(var, ...)                                                                          \
    if (mihatsu::_internal::rep_impl::breaking_state<>::check())                                   \
        MIHATSU_CONDITIONAL(MIHATSU_IS_BRACKETED(var), _MIHATSU_REPREV_BRACKETED, _MIHATSU_REPREV) \
    (var, __VA_ARGS__)
#define _MIHATSU_REPREV(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REPREV, __VA_ARGS__)
#define _MIHATSU_REPREV2(i, n) \
    if (auto&& _##i##_range = (n); true) _MIHATSU_REPREV_RANGE(__COUNTER__, mihatsu::_internal::rep_impl::range(_##i##_range), i)
#define _MIHATSU_REPREV3(i, a, b) for (std::common_type_t<decltype(a), decltype(b)> i = b - 1, _##i##_end = a - 1; i != _##i##_end; --i)
#define _MIHATSU_REPREV_BRACKETED(vars, ...) _MIHATSU_REPREV_RANGE(__COUNTER__, (__VA_ARGS__), _MIHATSU_REP_BRACKETED_VARS vars)
#define _MIHATSU_REPREV_RANGE(...) _MIHATSU_REPREV_RANGE_IMPL(__VA_ARGS__)
#define _MIHATSU_REPREV_RANGE_IMPL(id, range, ...)                                      \
    if (auto&& _reprev##id##_range = range; true)                                       \
        if (auto _reprev##id##_begin = std::rbegin(_reprev##id##_range); true)          \
            if (auto _reprev##id##_end = std::rend(_reprev##id##_range); true)          \
                for (; _reprev##id##_begin != _reprev##id##_end; ++_reprev##id##_begin) \
                    if (auto&& __VA_ARGS__ = *_reprev##id##_begin; true)

#define break(label)                                                \
    {                                                               \
        mihatsu::_internal::rep_impl::breaking_state<>::do_break(); \
        goto label;                                                 \
    }                                                               \
    ([]() {}())
