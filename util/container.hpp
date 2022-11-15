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
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "../internal/meta.hpp"

namespace mihatsu {

namespace _internal {

using signed_size_t = std::make_signed_t<std::size_t>;

template <typename T, typename = void>
struct container_selector {
    using set = typename std::set<T>;
    using multiset = typename std::multiset<T>;
    template <typename U>
    using map = typename std::map<T, U>;
    template <typename U>
    using multimap = std::multimap<T, U>;
};

template <typename T>
struct container_selector<T, std::void_t<decltype(std::hash<T>())>> {
    using set = typename std::unordered_set<T>;
    using multiset = typename std::unordered_multiset<T>;
    template <typename U>
    using map = typename std::unordered_map<T, U>;
    template <typename U>
    using multimap = std::unordered_multimap<T, U>;
};

namespace container_helper_base {

template <typename Container>
struct base {
    Container& container;
    inline base(Container& container)
        : container(container) {}

    using value_type = typename Container::value_type;

    inline signed_size_t size() const {
        return std::size(container);
    };

    template <typename Fn>
    using invoke_result_t = std::invoke_result_t<Fn, value_type>;
    template <typename Fn, typename Iterator>
    inline auto invoke(Fn&& fn, Iterator it) {
        return std::invoke(fn, *it);
    }

    inline void reserve(std::size_t) {}

    template <typename Iterator>
    inline auto get_value(Iterator it) const {
        return *it;
    }

    template <typename PositionIterator, typename Value>
    inline void insert(PositionIterator, Value&& val) {
        container.push_back(std::forward<Value>(val));
    }
};

template <typename RandomAccessContainer>
struct random_access : base<RandomAccessContainer> {
    using base<RandomAccessContainer>::base;

    using key_type = signed_size_t;
    using typename base<RandomAccessContainer>::value_type;

    template <typename Fn>
    using invoke_result_t = std::invoke_result_t<Fn, value_type, key_type>;
    template <typename Fn, typename Iterator>
    inline auto invoke(Fn&& fn, Iterator it) {
        return std::invoke(fn, *it, static_cast<signed_size_t>(it - std::begin(this->container)));
    }
};

template <typename Set>
struct set : base<Set> {
    using base<Set>::base;

    template <typename PositionIterator, typename Value>
    inline void insert(PositionIterator, Value&& val) {
        this->container.insert(std::forward<Value>(val));
    }
};

template <typename Map>
struct map : base<Map> {
    using base<Map>::base;

    using key_type = typename Map::key_type;
    using value_type = typename Map::mapped_type;

    template <typename Fn>
    using invoke_result_t = std::invoke_result_t<Fn, value_type, key_type>;
    template <typename Fn, typename Iterator>
    inline auto invoke(Fn&& fn, Iterator it) {
        return std::invoke(fn, it->second, it->first);
    }

    template <typename Iterator>
    inline auto get_value(Iterator it) const {
        return it->second;
    }

    template <typename PositionIterator, typename Value>
    inline void insert(PositionIterator pos, Value&& val) {
        this->container.emplace(pos->first, std::forward<Value>(val));
    }
};

}  // namespace container_helper_base

template <typename Container>
struct container_helper : container_helper_base::base<Container> {
    using container_helper_base::base<Container>::base;
};
template <typename Container>
container_helper(Container&) -> container_helper<Container>;

template <class T, class Allocator>
struct container_helper<std::vector<T, Allocator>> : container_helper_base::random_access<std::vector<T, Allocator>> {
    using container_helper_base::random_access<std::vector<T, Allocator>>::random_access;

    inline void reserve(std::size_t n) {
        this->container.reserve(n);
    }

    template <typename U>
    using change_value_type = typename std::vector<U>;
};

template <class T, std::size_t N>
struct container_helper<std::array<T, N>> : container_helper_base::random_access<std::array<T, N>> {
    using container_helper_base::random_access<std::array<T, N>>::random_access;

    std::size_t insertion_index = 0;
    template <typename PositionIterator, typename Value>
    inline void insert(PositionIterator, Value&& val) {
        this->container[insertion_index++] = std::forward<Value>(val);
    }

    template <typename U>
    using change_value_type = typename std::array<U, N>;
};

template <class T, class Allocator>
struct container_helper<std::deque<T, Allocator>> : container_helper_base::random_access<std::deque<T, Allocator>> {
    using container_helper_base::random_access<std::deque<T, Allocator>>::random_access;

    template <typename U>
    using change_value_type = typename std::deque<U>;
};

template <class T, class Allocator>
struct container_helper<std::list<T, Allocator>> : container_helper_base::base<std::list<T, Allocator>> {
    using container_helper_base::base<std::list<T, Allocator>>::base;

    template <typename U>
    using change_value_type = typename std::list<U>;
};

template <class T, class Allocator>
struct container_helper<std::forward_list<T, Allocator>> : container_helper_base::base<std::forward_list<T, Allocator>> {
    using List = std::forward_list<T, Allocator>;

    typename List::iterator inserting_iterator;

    inline container_helper(List& list)
        : container_helper_base::base<List>{list}, inserting_iterator(list.before_begin()) {}

    inline signed_size_t size() const {
        return -1;
    };

    template <typename PositionIterator, typename Value>
    inline void insert(PositionIterator, Value&& val) {
        this->container.insert_after(inserting_iterator, std::forward<Value>(val));
        ++inserting_iterator;
    }

    template <typename U>
    using change_value_type = typename std::forward_list<U>;
};

template <class T, class Compare, class Allocator>
struct container_helper<std::set<T, Compare, Allocator>> : container_helper_base::set<std::set<T, Compare, Allocator>> {
    using container_helper_base::set<std::set<T, Compare, Allocator>>::set;

    template <typename U>
    using change_value_type = typename container_selector<U>::set;
};

template <class T, class Compare, class Allocator>
struct container_helper<std::multiset<T, Compare, Allocator>> : container_helper_base::set<std::multiset<T, Compare, Allocator>> {
    using container_helper_base::set<std::multiset<T, Compare, Allocator>>::set;

    template <typename U>
    using change_value_type = typename container_selector<U>::multiset;
};

template <class Key, class Hash, class Pred, class Allocator>
struct container_helper<std::unordered_set<Key, Hash, Pred, Allocator>> : container_helper_base::set<std::unordered_set<Key, Hash, Pred, Allocator>> {
    using container_helper_base::set<std::unordered_set<Key, Hash, Pred, Allocator>>::set;

    template <typename U>
    using change_value_type = typename container_selector<U>::set;
};

template <class Key, class Hash, class Pred, class Allocator>
struct container_helper<std::unordered_multiset<Key, Hash, Pred, Allocator>> : container_helper_base::set<std::unordered_multiset<Key, Hash, Pred, Allocator>> {
    using container_helper_base::set<std::unordered_multiset<Key, Hash, Pred, Allocator>>::set;

    template <typename U>
    using change_value_type = typename container_selector<U>::multiset;
};

template <class Key, class T, class Compare, class Allocator>
struct container_helper<std::map<Key, T, Compare, Allocator>> : container_helper_base::map<std::map<Key, T, Compare, Allocator>> {
    using container_helper_base::map<std::map<Key, T, Compare, Allocator>>::map;

    template <typename U>
    using change_value_type = typename std::map<Key, U, Compare>;
};

template <class Key, class T, class Compare, class Allocator>
struct container_helper<std::multimap<Key, T, Compare, Allocator>> : container_helper_base::map<std::multimap<Key, T, Compare, Allocator>> {
    using container_helper_base::map<std::multimap<Key, T, Compare, Allocator>>::map;

    template <typename U>
    using change_value_type = typename std::multimap<Key, U, Compare>;
};

template <class Key, class T, class Hash, class Pred, class Allocator>
struct container_helper<std::unordered_map<Key, T, Hash, Pred, Allocator>> : container_helper_base::map<std::unordered_map<Key, T, Hash, Pred, Allocator>> {
    using container_helper_base::map<std::unordered_map<Key, T, Hash, Pred, Allocator>>::map;

    template <typename U>
    using change_value_type = typename std::unordered_map<Key, U, Hash, Pred>;
};

template <class Key, class T, class Hash, class Pred, class Allocator>
struct container_helper<std::unordered_multimap<Key, T, Hash, Pred, Allocator>> : container_helper_base::map<std::unordered_multimap<Key, T, Hash, Pred, Allocator>> {
    using container_helper_base::map<std::unordered_multimap<Key, T, Hash, Pred, Allocator>>::map;

    template <typename U>
    using change_value_type = typename std::unordered_multimap<Key, U, Hash, Pred>;
};

}  // namespace _internal

template <class Container, typename MappingFunction>
inline auto mapped(Container&& container, MappingFunction&& fn) {
    _internal::container_helper helper(container);

    using U = typename decltype(helper)::template invoke_result_t<MappingFunction>;
    typename decltype(helper)::template change_value_type<U> res;
    _internal::container_helper res_helper(res);

    if (helper.size() >= 0) res_helper.reserve(helper.size());
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        res_helper.insert(it, helper.invoke(fn, it));
    }

    return res;
}

template <class Container>
inline auto sorted(Container&& container) {
    _internal::container_helper helper(container);
    using T = typename decltype(helper)::value_type;

    std::vector<T> res;
    if (helper.size() >= 0) res.reserve(helper.size());
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        res.push_back(helper.get_value(it));
    }
    std::sort(res.begin(), res.end());
    return res;
}

template <class Container, typename KeyFunction>
inline auto sorted(Container&& container, KeyFunction&& fn) {
    _internal::container_helper helper(container);
    using T = typename decltype(helper)::value_type;
    using U = typename decltype(helper)::template invoke_result_t<KeyFunction>;
    using P = std::pair<T, U>;

    std::vector<P> temp;
    if (helper.size() >= 0) temp.reserve(helper.size());
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        temp.emplace_back(helper.get_value(it), helper.invoke(fn, it));
    }
    std::sort(temp.begin(), temp.end(), [](const P& lhs, const P& rhs) { return lhs.second < rhs.second; });

    std::vector<T> res;
    res.reserve(temp.size());
    for (auto&& [t, u] : temp) res.push_back(t);
    return res;
}

template <class Container>
inline auto ranked(Container&& container) {
    _internal::container_helper helper(container);
    using T = typename decltype(helper)::value_type;

    std::vector<T> vals;
    if (helper.size() >= 0) vals.reserve(helper.size());
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        vals.push_back(helper.get_value(it));
    }
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());

    return mapped(
        std::forward<Container>(container),
        [&vals](const T& val, _internal::signed_size_t _) -> _internal::signed_size_t {
            return std::lower_bound(vals.begin(), vals.end(), val) - vals.begin();
        });
}

template <class Container, typename KeyFunction>
inline auto ranked(Container&& container, KeyFunction&& fn) {
    _internal::container_helper helper(container);
    using T = typename decltype(helper)::value_type;
    using U = typename decltype(helper)::template invoke_result_t<KeyFunction>;

    std::vector<U> temp;
    if (helper.size() >= 0) temp.reserve(helper.size());
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        temp.push_back(helper.invoke(fn, it));
    }

    std::vector<U> keys(temp);
    std::sort(keys.begin(), keys.end());
    keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

    std::size_t i = 0;
    return mapped(
        std::forward<Container>(container),
        [&keys, &temp, &i](const T& val, _internal::signed_size_t _) -> _internal::signed_size_t {
            return std::lower_bound(keys.begin(), keys.end(), temp[i++]) - keys.begin();
        });
}

}  // namespace mihatsu
