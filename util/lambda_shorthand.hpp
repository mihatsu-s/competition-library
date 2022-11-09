#pragma once

#include <string>
#include <type_traits>

namespace mihatsu {
namespace lambda_shorthand {

namespace expression {

struct base {};

template <typename T>
constexpr inline bool is_expression = std::is_base_of_v<base, T>;

template <typename T>
struct constant : base {
    T value;
    constexpr inline constant(const T& value)
        : value(value) {}
    template <typename Env>
    constexpr inline T operator()(const Env& env) const {
        return value;
    }
};
constant(const char*)->constant<std::string>;

template <typename T, typename = void>
struct constant_t_impl {
    using type = constant<T>;
};
template <typename T>
struct constant_t_impl<T, std::enable_if_t<std::is_convertible_v<T, std::string>>> {
    using type = constant<std::string>;
};
template <typename T>
using constant_t = typename constant_t_impl<T>::type;

#define _MIHATSU_DEF_EXPR1(name, op, op_func)                                \
    template <typename Exp>                                                  \
    struct name : base {                                                     \
        Exp exp;                                                             \
        constexpr inline name(const Exp& exp)                                \
            : exp(exp) {}                                                    \
        template <typename Env>                                              \
        constexpr inline auto operator()(const Env& env) const {             \
            return op(exp(env));                                             \
        }                                                                    \
    };                                                                       \
    template <typename Exp, std::enable_if_t<is_expression<Exp>>* = nullptr> \
    constexpr inline name<Exp> op_func(const Exp& exp) {                     \
        return {exp};                                                        \
    }

_MIHATSU_DEF_EXPR1(plus, +, operator+);
_MIHATSU_DEF_EXPR1(minus, -, operator-);
_MIHATSU_DEF_EXPR1(lnot, !, operator!);

#undef _MIHATSU_DEF_EXPR1

#define _MIHATSU_DEF_EXPR2(name, op, op_func)                                                             \
    template <typename L, typename R>                                                                     \
    struct name : base {                                                                                  \
        L l;                                                                                              \
        R r;                                                                                              \
        constexpr inline name(const L& l, const R& r)                                                     \
            : l(l), r(r) {}                                                                               \
        template <typename Env>                                                                           \
        constexpr inline auto operator()(const Env& env) const {                                          \
            return l(env) op r(env);                                                                      \
        }                                                                                                 \
    };                                                                                                    \
    template <typename L, typename R, std::enable_if_t<is_expression<L> && is_expression<R>>* = nullptr>  \
    constexpr inline name<L, R> op_func(const L& l, const R& r) {                                         \
        return {l, r};                                                                                    \
    }                                                                                                     \
    template <typename L, typename R, std::enable_if_t<!is_expression<L> && is_expression<R>>* = nullptr> \
    constexpr inline name<constant_t<L>, R> op_func(const L& l, const R& r) {                             \
        return {constant_t<L>(l), r};                                                                     \
    }                                                                                                     \
    template <typename L, typename R, std::enable_if_t<is_expression<L> && !is_expression<R>>* = nullptr> \
    constexpr inline name<L, constant_t<R>> op_func(const L& l, const R& r) {                             \
        return {l, constant_t<R>(r)};                                                                     \
    }

_MIHATSU_DEF_EXPR2(add, +, operator+)
_MIHATSU_DEF_EXPR2(sub, -, operator-)
_MIHATSU_DEF_EXPR2(mult, *, operator*)
_MIHATSU_DEF_EXPR2(div, /, operator/)
_MIHATSU_DEF_EXPR2(mod, %, operator%)
_MIHATSU_DEF_EXPR2(land, &&, operator&&)
_MIHATSU_DEF_EXPR2(lor, ||, operator||)
_MIHATSU_DEF_EXPR2(eq, ==, operator==)
_MIHATSU_DEF_EXPR2(ne, !=, operator!=)
_MIHATSU_DEF_EXPR2(lt, <, operator<)
_MIHATSU_DEF_EXPR2(le, <=, operator<=)
_MIHATSU_DEF_EXPR2(gt, >, operator>)
_MIHATSU_DEF_EXPR2(ge, >=, operator>=)

#undef _MIHATSU_DEF_EXPR2

template <typename Cond, typename T, typename F>
struct conditional : base {
    Cond cond;
    T t;
    F f;
    constexpr inline conditional(const Cond& cond, const T& t, const F& f)
        : cond(cond), t(t), f(f) {}
    template <typename Env>
    constexpr inline auto operator()(const Env& env) const {
        return cond(env) ? t(env) : f(env);
    }
};
template <typename Cond, typename T>
struct conditional_inter {  // not expression
    Cond cond;
    T t;
    constexpr inline conditional_inter(const Cond& cond, const T& t)
        : cond(cond), t(t) {}
    template <typename F, std::enable_if_t<is_expression<F>>* = nullptr>
    constexpr inline conditional<Cond, T, F> operator|(const F& f) {
        return {cond, t, f};
    }
    template <typename F, std::enable_if_t<!is_expression<F>>* = nullptr>
    constexpr inline conditional<Cond, T, constant_t<F>> operator|(const F& f) {
        return {cond, t, constant_t<F>(f)};
    }
};
template <typename Cond, typename T, std::enable_if_t<is_expression<Cond> && is_expression<T>>* = nullptr>
constexpr inline conditional_inter<Cond, T> operator|(const Cond& cond, const T& t) {
    return {cond, t};
}
template <typename Cond, typename T, std::enable_if_t<is_expression<Cond> && !is_expression<T>>* = nullptr>
constexpr inline conditional_inter<Cond, constant_t<T>> operator|(const Cond& cond, const T& t) {
    return {cond, constant_t<T>(t)};
}

}  // namespace expression

struct variable : expression::base {
    template <typename Env>
    constexpr inline Env operator()(const Env& env) const {
        return env;
    }
};

}  // namespace lambda_shorthand
}  // namespace mihatsu
