#pragma once

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "../internal/meta.hpp"

namespace mihatsu {

template <typename T, typename U>
inline T& chmax(T& x, U&& y) {
    return (x < y) ? (x = std::forward<U>(y)) : x;
}
template <typename T, typename U>
inline T& chmin(T& x, U&& y) {
    return (y < x) ? (x = std::forward<U>(y)) : x;
}

namespace _internal {

template <typename T, typename = void>
struct container_selector {
    using set = typename std::set<T>;
    template <typename U>
    using map = typename std::map<T, U>;
};

template <typename T>
struct container_selector<T, std::void_t<decltype(std::hash<T>())>> {
    using set = typename std::unordered_set<T>;
    template <typename U>
    using map = typename std::unordered_map<T, U>;
};

using signed_size_t = std::make_signed_t<std::size_t>;

}  // namespace _internal

template <class T, class Allocator, typename Mapping>
inline auto mapped(const std::vector<T, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, _internal::signed_size_t>;
    std::vector<U> res;
    res.reserve(x.size());
    for (_internal::signed_size_t i = 0; i < x.size(); ++i) res.push_back(std::invoke(f, x[i], i));
    return res;
}

template <class T, std::size_t N, typename Mapping>
inline auto mapped(const std::array<T, N>& array, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, _internal::signed_size_t>;
    std::array<U, N> res;
    for (_internal::signed_size_t i = 0; i < N; ++i) res[i] = std::invoke(f, array[i], i);
    return res;
}

template <class T, class Allocator, typename Mapping>
inline auto mapped(const std::deque<T, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, _internal::signed_size_t>;
    std::deque<U> res;
    for (_internal::signed_size_t i = 0; i < x.size(); ++i) res.push_back(std::invoke(f, x[i], i));
    return res;
}

template <class T, class Allocator, typename Mapping>
inline auto mapped(const std::list<T, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, _internal::signed_size_t>;
    std::list<U> res;
    _internal::signed_size_t i = 0;
    for (auto&& v : x) res.push_back(std::invoke(f, v, i++));
    return res;
}

template <class T, class Allocator, typename Mapping>
inline auto mapped(const std::forward_list<T, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, _internal::signed_size_t>;
    std::forward_list<U> res;
    _internal::signed_size_t i = 0;
    for (auto&& v : x) res.push_front(std::invoke(f, v, i++));
    res.reverse();
    return res;
}

template <class Key, class T, class Compare, class Allocator, typename Mapping>
inline auto mapped(const std::map<Key, T, Compare, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, Key>;
    std::map<Key, U, Compare> res;
    for (auto&& [k, v] : x) res.emplace(k, std::invoke(f, v, k));
    return res;
}

template <class Key, class T, class Compare, class Allocator, typename Mapping>
inline auto mapped(const std::multimap<Key, T, Compare, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, Key>;
    std::multimap<Key, U, Compare, Allocator> res;
    for (auto&& [k, v] : x) res.emplace(k, std::invoke(f, v, k));
    return res;
}

template <class Key, class T, class Hash, class Pred, class Allocator, typename Mapping>
inline auto mapped(const std::unordered_map<Key, T, Hash, Pred, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, Key>;
    std::unordered_map<Key, U, Hash, Pred> res;
    for (auto&& [k, v] : x) res.emplace(k, std::invoke(f, v, k));
    return res;
}

template <class Key, class T, class Hash, class Pred, class Allocator, typename Mapping>
inline auto mapped(const std::unordered_multimap<Key, T, Hash, Pred, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, T, Key>;
    std::unordered_multimap<Key, U, Hash, Pred> res;
    for (auto&& [k, v] : x) res.emplace(k, std::invoke(f, v, k));
    return res;
}

template <class Key, class Compare, class Allocator, typename Mapping>
inline auto mapped(const std::set<Key, Compare, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, Key>;
    std::set<U> res;
    for (auto&& v : x) res.insert(std::invoke(f, v));
    return res;
}

template <class Key, class Compare, class Allocator, typename Mapping>
inline auto mapped(const std::multiset<Key, Compare, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, Key>;
    std::multiset<U> res;
    for (auto&& v : x) res.insert(std::invoke(f, v));
    return res;
}

template <class Key, class Hash, class Pred, class Allocator, typename Mapping>
inline auto mapped(const std::unordered_set<Key, Hash, Pred, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, Key>;
    std::unordered_set<U> res;
    for (auto&& v : x) res.insert(std::invoke(f, v));
    return res;
}

template <class Key, class Hash, class Pred, class Allocator, typename Mapping>
inline auto mapped(const std::unordered_multiset<Key, Hash, Pred, Allocator>& x, Mapping f) {
    using U = std::invoke_result_t<Mapping, Key>;
    std::unordered_multiset<U> res;
    for (auto&& v : x) res.insert(std::invoke(f, v));
    return res;
}

template <typename Container, std::enable_if_t<_internal::is_container_v<Container>>* = nullptr>
inline auto sorted(Container&& container) {
    using T = std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(container))>>;
    std::vector<T> res(std::begin(container), std::end(container));
    std::sort(res.begin(), res.end());
    return res;
}

template <typename Container, typename KeyFunc, std::enable_if_t<_internal::is_container_v<Container>>* = nullptr>
inline auto sorted(Container&& container, KeyFunc f) {
    using T = std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(container))>>;
    using Key = std::invoke_result_t<KeyFunc, T, _internal::signed_size_t>;
    std::vector<std::tuple<Key, _internal::signed_size_t, T>> temp;
    temp.reserve(std::size(container));
    _internal::signed_size_t i = 0;
    for (auto&& v : container) temp.emplace_back(std::invoke(f, v, i), i++, v);
    std::sort(temp.begin(), temp.end());
    std::vector<T> res;
    res.reserve(std::size(container));
    for (auto&& [k, i, v] : temp) res.push_back(v);
    return res;
}

}  // namespace mihatsu
