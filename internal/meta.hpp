#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mihatsu::_internal {

// ---- constant mappings ----

template <typename>
constexpr inline bool false_v = false;
template <typename>
constexpr inline bool true_v = true;

// ---- type traits ----

template <typename, typename = void>
struct is_maplike : std::false_type {};
template <typename T>
struct is_maplike<T, std::void_t<typename T::mapped_type>> : std::true_type {};
template <typename T>
constexpr inline bool is_maplike_v = is_maplike<T>::value;

template <typename, typename = void>
struct is_setlike : std::false_type {};
template <typename T>
struct is_setlike<T, std::enable_if_t<std::is_same_v<typename T::key_type, typename T::value_type>>> : std::true_type {};
template <typename T>
constexpr inline bool is_setlike_v = is_setlike<T>::value;

template <typename, typename = void>
struct is_container : std::false_type {};
template <typename T>
struct is_container<T, std::void_t<decltype(std::begin(std::declval<T>()))>> : std::true_type {};
template <typename T>
constexpr inline bool is_container_v = is_container<T>::value;

template <typename, typename = void>
struct is_pairlike : std::false_type {};
template <typename T>
struct is_pairlike<T, std::void_t<typename T::first_type, typename T::second_type>> : std::true_type {};
template <typename T>
constexpr inline bool is_pairlike_v = is_pairlike<T>::value;

struct is_subscriptable_impl {
    template <class T, class U>
    constexpr auto operator()(T&& a, U&& b) -> decltype(a[b], std::true_type{}) {
        return std::true_type{};
    }
    constexpr auto operator()(...) -> std::false_type {
        return std::false_type{};
    }
};
template <class T, class U>
constexpr inline bool is_subscriptable_v = std::invoke_result_t<is_subscriptable_impl, T, U>::value;

// ---- utilities ----

template <typename T, std::size_t N>
struct arraylike_tuple {
    template <typename>
    struct impl {};
    template <std::size_t... I>
    struct impl<std::index_sequence<I...>> {
        template <std::size_t>
        using T_ = T;
        using type = std::tuple<T_<I>...>;
    };
    using type = typename impl<std::make_index_sequence<N>>::type;
};
template <typename T, std::size_t N>
using arraylike_tuple_t = typename arraylike_tuple<T, N>::type;

}  // namespace mihatsu::_internal
