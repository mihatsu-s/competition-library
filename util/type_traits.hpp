#ifndef MIHATSU_UTIL_TYPE_TRAITS_HPP
#define MIHATSU_UTIL_TYPE_TRAITS_HPP 1

#include <iterator>
#include <type_traits>

namespace mihatsu {
namespace _internal {

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

template <typename T, typename = void>
struct is_pairlike : std::false_type {};
template <typename T>
struct is_pairlike<T, std::void_t<typename T::first_type, typename T::second_type>> : std::true_type {};
template <typename T>
constexpr inline bool is_pairlike_v = is_pairlike<T>::value;

}  // namespace _internal
}  // namespace mihatsu

#endif
