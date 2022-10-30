#ifndef MIHATSU_UTIL_TYPE_TRAITS_HPP
#define MIHATSU_UTIL_TYPE_TRAITS_HPP 1

#include <iterator>
#include <type_traits>

namespace mihatsu {
namespace _internal {

#define MIHATSU_DEFINE_TYPE_CHECKER(name, ...)       \
    template <typename T, typename _ = void>         \
    struct name : std::false_type {};                \
    template <typename T>                            \
    struct name<T, __VA_ARGS__> : std::true_type {}; \
    template <typename T>                            \
    inline constexpr bool name##_v = name<T>::value;

MIHATSU_DEFINE_TYPE_CHECKER(is_maplike, std::void_t<typename T::mapped_type>)
MIHATSU_DEFINE_TYPE_CHECKER(is_container, std::void_t<decltype(std::begin(std::declval<T>()))>)
MIHATSU_DEFINE_TYPE_CHECKER(is_atcoder_modint, std::enable_if_t<std::is_same_v<decltype(std::declval<T>().inv().val()), unsigned int>>)

#undef MIHATSU_DEFINE_TYPE_CHECKER

}  // namespace _internal
}  // namespace mihatsu

#endif
