#pragma once

#include <functional>
#include <type_traits>

namespace mihatsu::_internal {

// 仮実装（TODO: 任意引数長に対応する）

struct {
    template <class Fn>
    inline constexpr auto operator()(Fn&& fn) {
        return std::invoke(std::forward<Fn>(fn));
    }

    template <class Fn, class T1>
    inline constexpr auto operator()(Fn&& fn, T1&& t1) {
        if constexpr (std::is_invocable_v<Fn, T1>) {
            return std::invoke(std::forward<Fn>(fn), std::forward<T1>(t1));
        } else {
            return this->operator()(std::forward<Fn>(fn));
        }
    }

    template <class Fn, class T1, class T2>
    inline constexpr auto operator()(Fn&& fn, T1&& t1, T2&& t2) {
        if constexpr (std::is_invocable_v<Fn, T1, T2>) {
            return std::invoke(std::forward<Fn>(fn), std::forward<T1>(t1), std::forward<T2>(t2));
        } else {
            return this->operator()(std::forward<Fn>(fn), std::forward<T1>(t1));
        }
    }

    template <class Fn, class T1, class T2, class T3>
    inline constexpr auto operator()(Fn&& fn, T1&& t1, T2&& t2, T3&& t3) {
        if constexpr (std::is_invocable_v<Fn, T1, T2, T3>) {
            return std::invoke(std::forward<Fn>(fn), std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3));
        } else {
            return this->operator()(std::forward<Fn>(fn), std::forward<T1>(t1), std::forward<T2>(t2));
        }
    }

    template <class Fn, class T1, class T2, class T3, class T4>
    inline constexpr auto operator()(Fn&& fn, T1&& t1, T2&& t2, T3&& t3, T4&& t4) {
        if constexpr (std::is_invocable_v<Fn, T1, T2, T3, T4>) {
            return std::invoke(std::forward<Fn>(fn), std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3), std::forward<T4>(t4));
        } else {
            return this->operator()(std::forward<Fn>(fn), std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3));
        }
    }
} redundant_invoke;

template <class Fn, class... Args>
constexpr inline bool is_redundant_invocable_v = std::is_invocable_v<decltype(redundant_invoke), Fn, Args...>;

template <class Fn, class... Args>
using redundant_invoke_result_t = typename std::invoke_result_t<decltype(redundant_invoke), Fn, Args...>;

}  // namespace mihatsu::_internal
