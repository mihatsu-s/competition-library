#pragma once

#include <string>
#include <tuple>
#include <type_traits>

namespace mihatsu {
namespace lambda_shorthand {

namespace expression {

// ---- base ----

struct base {};

template <typename T>
constexpr inline bool is_expression = std::is_base_of_v<base, T>;

// ---- constant ----

template <typename T>
struct constant : base {
    T value;
    constexpr inline constant(const T& value)
        : value(value) {}
    template <typename... Args>
    constexpr inline T operator()(const Args&... args) const {
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

// ---- variable ----

template <std::size_t ID>
struct variable : base {
    template <typename... Args>
    constexpr inline auto operator()(const Args&... args) const {
        return std::get<ID>(std::make_tuple(args...));
    }

    struct : base {
        template <typename... Args>
        constexpr inline auto operator()(const Args&... args) const {
            return std::get<ID>(std::make_tuple(args...)).first;
        }
        struct : base {
            template <typename... Args>
            constexpr inline auto operator()(const Args&... args) const {
                return std::get<ID>(std::make_tuple(args...)).first.first;
            }
        } first;
        struct : base {
            template <typename... Args>
            constexpr inline auto operator()(const Args&... args) const {
                return std::get<ID>(std::make_tuple(args...)).first.second;
            }
        } second;
    } first;
    struct : base {
        template <typename... Args>
        constexpr inline auto operator()(const Args&... args) const {
            return std::get<ID>(std::make_tuple(args...)).second;
        }
        struct : base {
            template <typename... Args>
            constexpr inline auto operator()(const Args&... args) const {
                return std::get<ID>(std::make_tuple(args...)).second.first;
            }
        } first;
        struct : base {
            template <typename... Args>
            constexpr inline auto operator()(const Args&... args) const {
                return std::get<ID>(std::make_tuple(args...)).second.second;
            }
        } second;
    } second;
};

// ---- operators ----

#define _MIHATSU_DEF_EXPR1(name, op, op_func)                                  \
    template <typename Expr>                                                   \
    struct name : base {                                                       \
        Expr expr;                                                             \
        constexpr inline name(const Expr& expr)                                \
            : expr(expr) {}                                                    \
        template <typename... Args>                                            \
        constexpr inline auto operator()(const Args&... args) const {          \
            return op(expr(args...));                                          \
        }                                                                      \
    };                                                                         \
    template <typename Expr, std::enable_if_t<is_expression<Expr>>* = nullptr> \
    constexpr inline auto op_func(const Expr& expr) {                          \
        return name<Expr>(expr);                                               \
    }

_MIHATSU_DEF_EXPR1(plus, +, operator+);
_MIHATSU_DEF_EXPR1(minus, -, operator-);
_MIHATSU_DEF_EXPR1(lnot, !, operator!);
_MIHATSU_DEF_EXPR1(bnot, ~, operator~);

#define _MIHATSU_DEF_EXPR_OP2(name, op_func)                                                                      \
    template <typename Lhs, typename Rhs, std::enable_if_t<is_expression<Lhs> && is_expression<Rhs>>* = nullptr>  \
    constexpr inline auto op_func(const Lhs& lhs, const Rhs& rhs) {                                               \
        return name<Lhs, Rhs>(lhs, rhs);                                                                          \
    }                                                                                                             \
    template <typename Lhs, typename Rhs, std::enable_if_t<!is_expression<Lhs> && is_expression<Rhs>>* = nullptr> \
    constexpr inline auto op_func(const Lhs& lhs, const Rhs& rhs) {                                               \
        return name<constant_t<Lhs>, Rhs>(constant_t<Lhs>(lhs), rhs);                                             \
    }                                                                                                             \
    template <typename Lhs, typename Rhs, std::enable_if_t<is_expression<Lhs> && !is_expression<Rhs>>* = nullptr> \
    constexpr inline auto op_func(const Lhs& lhs, const Rhs& rhs) {                                               \
        return name<Lhs, constant_t<Rhs>>(lhs, constant_t<Rhs>(rhs));                                             \
    }

#define _MIHATSU_DEF_EXPR2(name, op, op_func)                         \
    template <typename Lhs, typename Rhs>                             \
    struct name : base {                                              \
        Lhs lhs;                                                      \
        Rhs rhs;                                                      \
        constexpr inline name(const Lhs& lhs, const Rhs& rhs)         \
            : lhs(lhs), rhs(rhs) {}                                   \
        template <typename... Args>                                   \
        constexpr inline auto operator()(const Args&... args) const { \
            return lhs(args...) op rhs(args...);                      \
        }                                                             \
    };                                                                \
    _MIHATSU_DEF_EXPR_OP2(name, op_func)

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
_MIHATSU_DEF_EXPR2(band, &, operator&)
// _MIHATSU_DEF_EXPR2(bor, |, operator|)  // '|' is conditional operator
_MIHATSU_DEF_EXPR2(bxor, ^, operator^)
_MIHATSU_DEF_EXPR2(lshift, <<, operator<<)
_MIHATSU_DEF_EXPR2(rshift, >>, operator>>)

// ---- conditional ----

template <typename Cond, typename T, typename F>
struct conditional : base {
    Cond cond;
    T t;
    F f;
    constexpr inline conditional(const Cond& cond, const T& t, const F& f)
        : cond(cond), t(t), f(f) {}
    template <typename... Args>
    constexpr inline auto operator()(const Args&... args) const {
        return cond(args...) ? t(args...) : f(args...);
    }
};

template <typename Cond, typename T>
struct conditional_intermediate {
    Cond cond;
    T t;
    constexpr inline conditional_intermediate(const Cond& cond, const T& t)
        : cond(cond), t(t) {}
    template <typename F, std::enable_if_t<is_expression<F>>* = nullptr>
    constexpr inline auto operator|(const F& f) {
        return conditional<Cond, T, F>(cond, t, f);
    }
    template <typename F, std::enable_if_t<!is_expression<F>>* = nullptr>
    constexpr inline auto operator|(const F& f) {
        return conditional<Cond, T, constant_t<F>>(cond, t, constant_t<F>(f));
    }
};

_MIHATSU_DEF_EXPR_OP2(conditional_intermediate, operator|)

// ---- pair ----

template <typename T1, typename T2>
struct pair : base {
    T1 first;
    T2 second;
    constexpr inline pair(const T1& first, const T2& second)
        : first(first), second(second) {}
    template <typename... Args>
    constexpr inline auto operator()(const Args&... args) const {
        return std::make_pair(first(args...), second(args...));
    }
};
#define _MIHATSU_COMMA ,
_MIHATSU_DEF_EXPR_OP2(pair, operator _MIHATSU_COMMA)

#undef _MIHATSU_COMMA
#undef _MIHATSU_DEF_EXPR1
#undef _MIHATSU_DEF_EXPR2
#undef _MIHATSU_DEF_EXPR_OP2

}  // namespace expression

template <std::size_t VariableID = 0>
constexpr inline auto make_variable() {
    return expression::variable<VariableID>();
}

template <typename T>
constexpr inline auto make_constant(const T& value) {
    return expression::constant<T>(value);
}

}  // namespace lambda_shorthand
}  // namespace mihatsu
