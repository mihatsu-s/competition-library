#ifndef MIHATSU_UTIL_HASH_HPP
#define MIHATSU_UTIL_HASH_HPP 1

#include <functional>
#include <tuple>
#include <utility>

namespace mihatsu {
namespace _internal {

std::size_t hash_sequence() {
    return 0;
}

template <typename Head, typename... Tail>
std::size_t hash_sequence(const Head& head, const Tail&... tail) {
    return std::hash<Head>()(head) ^ hash_sequence(tail...);
}

}  // namespace _internal
}  // namespace mihatsu

namespace std {

template <typename T1, typename T2>
class hash<std::pair<T1, T2>> {
   public:
    std::size_t operator()(const std::pair<T1, T2>& x) const {
        return mihatsu::_internal::hash_sequence(x.first, x.second);
    }
};

template <typename... T>
class hash<std::tuple<T...>> {
   public:
    std::size_t operator()(const std::tuple<T...>& x) const {
        return std::apply(mihatsu::_internal::hash_sequence<T...>, x);
    }
};

}  // namespace std

#endif
