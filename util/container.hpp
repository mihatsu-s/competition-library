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
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "../internal/meta.hpp"
#include "../internal/redundant_invoke.hpp"

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

struct {
    template <typename Fn, class First, class... Args>
    inline auto operator()(Fn&& fn, First&& first, Args&&... args) {
        if constexpr (is_redundant_invocable_v<Fn, First, Args...>) {
            return redundant_invoke(std::forward<Fn>(fn), std::forward<First>(first), std::forward<Args>(args)...);
        } else {
            return std::forward<Fn>(fn)[std::forward<First>(first)];
        }
    }
} container_invoke;

template <typename Fn, class First, class... Args>
using container_invoke_result_t = typename std::invoke_result_t<decltype(container_invoke), Fn, First, Args...>;

template <typename Container, bool is_const>
struct base {
    static_assert(!std::is_const_v<Container>, "parameter 'Container' cannot be const (use is_const=true instead)");

   protected:
    using _Container = typename std::conditional_t<is_const, const Container, Container>;

   public:
    _Container& container;
    inline base(_Container& container)
        : container(container) {}

    inline signed_size_t size() const {
        return std::size(container);
    };

    inline void reserve(std::size_t) {}

    static constexpr bool subscriptable = false;

    using value_type = typename _Container::value_type;
    template <typename Iterator>
    inline value_type get_value(Iterator it) const {
        return *it;
    }
    template <typename Iterator, typename Value, std::enable_if_t<true_v<Value> && !is_const>* = nullptr>
    inline void set_value(Iterator it, Value&& val) {
        *it = std::forward<Value>(val);
    }
    static constexpr bool value_settable = !is_const;

    using iterated_type = typename std::remove_reference_t<decltype(*std::begin(container))>;

    template <typename Fn>
    using invoke_result_t = container_invoke_result_t<Fn, value_type>;
    template <typename Fn, typename Iterator>
    inline auto invoke(Fn&& fn, Iterator it) {
        return container_invoke(fn, *it);
    }

    template <typename PositionIterator, typename Value, std::enable_if_t<true_v<Value> && !is_const>* = nullptr>
    inline void insert(PositionIterator, Value&& val) {
        // PositionIterator is not read except for map
        container.push_back(std::forward<Value>(val));
    }

    template <typename U>
    using change_value_type = typename std::vector<U>;
};

template <typename RandomAccessContainer, bool is_const>
struct random_access : base<RandomAccessContainer, is_const> {
    using base<RandomAccessContainer, is_const>::base;

    static constexpr bool subscriptable = true;
    using key_type = signed_size_t;
    template <typename Iterator>
    inline key_type get_key(Iterator it) const {
        return it - std::begin(this->container);
    }

    using typename base<RandomAccessContainer, is_const>::value_type;

    template <typename Fn>
    using invoke_result_t = container_invoke_result_t<Fn, value_type, key_type>;
    template <typename Fn, typename Iterator>
    inline auto invoke(Fn&& fn, Iterator it) {
        return container_invoke(fn, *it, static_cast<signed_size_t>(it - std::begin(this->container)));
    }
};

template <typename Set, bool is_const>
struct set : base<Set, is_const> {
    using base<Set, is_const>::base;

    static constexpr bool value_settable = false;

    template <typename PositionIterator, typename Value, std::enable_if_t<true_v<Value> && !is_const>* = nullptr>
    inline void insert(PositionIterator, Value&& val) {
        this->container.insert(std::forward<Value>(val));
    }
};

template <typename Map, bool is_const>
struct map : base<Map, is_const> {
    using base<Map, is_const>::base;

    static constexpr bool subscriptable = true;
    using key_type = typename Map::key_type;
    template <typename Iterator>
    inline key_type get_key(Iterator it) const {
        return it->first;
    }

    using value_type = typename Map::mapped_type;
    template <typename Iterator>
    inline value_type get_value(Iterator it) const {
        return it->second;
    }
    template <typename Iterator, typename Value, std::enable_if_t<true_v<Value> && !is_const>* = nullptr>
    inline void set_value(Iterator it, Value&& val) {
        it->second = std::forward<Value>(val);
    }

    template <typename Fn>
    using invoke_result_t = container_invoke_result_t<Fn, value_type, key_type>;
    template <typename Fn, typename Iterator>
    inline auto invoke(Fn&& fn, Iterator it) {
        return container_invoke(fn, it->second, it->first);
    }

    template <typename PositionIterator, typename Value, std::enable_if_t<true_v<Value> && !is_const>* = nullptr>
    inline void insert(PositionIterator pos, Value&& val) {
        this->container.emplace(pos->first, std::forward<Value>(val));
    }
};

}  // namespace container_helper_base

template <typename Container, bool is_const>
struct container_helper : container_helper_base::base<Container, is_const> {
    using container_helper_base::base<Container, is_const>::base;
};
template <typename Container>
container_helper(Container&) -> container_helper<Container, false>;
template <typename Container>
container_helper(const Container&) -> container_helper<Container, true>;

template <class T, class Allocator, bool is_const>
struct container_helper<std::vector<T, Allocator>, is_const> : container_helper_base::random_access<std::vector<T, Allocator>, is_const> {
    using container_helper_base::random_access<std::vector<T, Allocator>, is_const>::random_access;

    inline void reserve(std::size_t n) {
        this->container.reserve(n);
    }

    using iterated_type = T;
};

template <class charT, class traits, class Allocator, bool is_const>
struct container_helper<std::basic_string<charT, traits, Allocator>, is_const> : container_helper_base::random_access<std::basic_string<charT, traits, Allocator>, is_const> {
    using container_helper_base::random_access<std::basic_string<charT, traits, Allocator>, is_const>::random_access;

    using iterated_type = charT;

    template <typename U>
    using change_value_type = typename std::conditional_t<std::is_same_v<U, charT>, std::basic_string<charT>, std::vector<U>>;
};

template <class T, std::size_t N, bool is_const>
struct container_helper<std::array<T, N>, is_const> : container_helper_base::random_access<std::array<T, N>, is_const> {
    using container_helper_base::random_access<std::array<T, N>, is_const>::random_access;

    using iterated_type = T;

    std::size_t insertion_index = 0;
    template <typename PositionIterator, typename Value>
    inline void insert(PositionIterator, Value&& val) {
        this->container[insertion_index++] = std::forward<Value>(val);
    }

    template <typename U>
    using change_value_type = typename std::array<U, N>;
};

template <class T, class Allocator, bool is_const>
struct container_helper<std::deque<T, Allocator>, is_const> : container_helper_base::random_access<std::deque<T, Allocator>, is_const> {
    using container_helper_base::random_access<std::deque<T, Allocator>, is_const>::random_access;

    using iterated_type = T;

    template <typename U>
    using change_value_type = typename std::deque<U>;
};

template <class T, class Allocator, bool is_const>
struct container_helper<std::list<T, Allocator>, is_const> : container_helper_base::base<std::list<T, Allocator>, is_const> {
    using container_helper_base::base<std::list<T, Allocator>, is_const>::base;

    using iterated_type = T;

    template <typename U>
    using change_value_type = typename std::list<U>;
};

template <class T, class Allocator>
struct container_helper<std::forward_list<T, Allocator>, false> : container_helper_base::base<std::forward_list<T, Allocator>, false> {
   protected:
    using List = typename std::forward_list<T, Allocator>;
    typename List::iterator inserting_iterator;

   public:
    inline container_helper(List& list)
        : container_helper_base::base<List, false>{list}, inserting_iterator(list.before_begin()) {}

    using iterated_type = T;

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
template <class T, class Allocator>
struct container_helper<std::forward_list<T, Allocator>, true> : container_helper_base::base<std::forward_list<T, Allocator>, true> {
    using container_helper_base::base<std::forward_list<T, Allocator>, true>::base;

    using iterated_type = T;

    inline signed_size_t size() const {
        return -1;
    };

    template <typename U>
    using change_value_type = typename std::forward_list<U>;
};

template <class T, class Compare, class Allocator, bool is_const>
struct container_helper<std::set<T, Compare, Allocator>, is_const> : container_helper_base::set<std::set<T, Compare, Allocator>, is_const> {
    using container_helper_base::set<std::set<T, Compare, Allocator>, is_const>::set;

    using iterated_type = T;

    template <typename U>
    using change_value_type = typename container_selector<U>::set;
};

template <class T, class Compare, class Allocator, bool is_const>
struct container_helper<std::multiset<T, Compare, Allocator>, is_const> : container_helper_base::set<std::multiset<T, Compare, Allocator>, is_const> {
    using container_helper_base::set<std::multiset<T, Compare, Allocator>, is_const>::set;

    using iterated_type = T;

    template <typename U>
    using change_value_type = typename container_selector<U>::multiset;
};

template <class Key, class Hash, class Pred, class Allocator, bool is_const>
struct container_helper<std::unordered_set<Key, Hash, Pred, Allocator>, is_const> : container_helper_base::set<std::unordered_set<Key, Hash, Pred, Allocator>, is_const> {
    using container_helper_base::set<std::unordered_set<Key, Hash, Pred, Allocator>, is_const>::set;

    using iterated_type = Key;

    template <typename U>
    using change_value_type = typename container_selector<U>::set;
};

template <class Key, class Hash, class Pred, class Allocator, bool is_const>
struct container_helper<std::unordered_multiset<Key, Hash, Pred, Allocator>, is_const> : container_helper_base::set<std::unordered_multiset<Key, Hash, Pred, Allocator>, is_const> {
    using container_helper_base::set<std::unordered_multiset<Key, Hash, Pred, Allocator>, is_const>::set;

    using iterated_type = Key;

    template <typename U>
    using change_value_type = typename container_selector<U>::multiset;
};

template <class Key, class T, class Compare, class Allocator, bool is_const>
struct container_helper<std::map<Key, T, Compare, Allocator>, is_const> : container_helper_base::map<std::map<Key, T, Compare, Allocator>, is_const> {
    using container_helper_base::map<std::map<Key, T, Compare, Allocator>, is_const>::map;

    using iterated_type = std::pair<Key, T>;

    template <typename U>
    using change_value_type = typename std::map<Key, U, Compare>;
};

template <class Key, class T, class Compare, class Allocator, bool is_const>
struct container_helper<std::multimap<Key, T, Compare, Allocator>, is_const> : container_helper_base::map<std::multimap<Key, T, Compare, Allocator>, is_const> {
    using container_helper_base::map<std::multimap<Key, T, Compare, Allocator>, is_const>::map;

    using iterated_type = std::pair<Key, T>;

    template <typename U>
    using change_value_type = typename std::multimap<Key, U, Compare>;
};

template <class Key, class T, class Hash, class Pred, class Allocator, bool is_const>
struct container_helper<std::unordered_map<Key, T, Hash, Pred, Allocator>, is_const> : container_helper_base::map<std::unordered_map<Key, T, Hash, Pred, Allocator>, is_const> {
    using container_helper_base::map<std::unordered_map<Key, T, Hash, Pred, Allocator>, is_const>::map;

    using iterated_type = std::pair<Key, T>;

    template <typename U>
    using change_value_type = typename std::unordered_map<Key, U, Hash, Pred>;
};

template <class Key, class T, class Hash, class Pred, class Allocator, bool is_const>
struct container_helper<std::unordered_multimap<Key, T, Hash, Pred, Allocator>, is_const> : container_helper_base::map<std::unordered_multimap<Key, T, Hash, Pred, Allocator>, is_const> {
    using container_helper_base::map<std::unordered_multimap<Key, T, Hash, Pred, Allocator>, is_const>::map;

    using iterated_type = std::pair<Key, T>;

    template <typename U>
    using change_value_type = typename std::unordered_multimap<Key, U, Hash, Pred>;
};

}  // namespace _internal

template <class Container, typename MappingFunction>
inline auto mapped(Container&& container, MappingFunction&& fn) {
    _internal::container_helper helper(container);

    using U = typename decltype(helper)::template invoke_result_t<MappingFunction>;
    using ResultType = typename decltype(helper)::template change_value_type<U>;

    if constexpr (std::is_base_of_v<ResultType, Container /* rvalue */> && !std::is_const_v<Container> && decltype(helper)::value_settable) {
        for (auto it = std::begin(container); it != std::end(container); ++it) {
            helper.set_value(it, helper.invoke(fn, it));
        }
        return container;
    } else {
        ResultType res;
        _internal::container_helper res_helper(res);
        if (helper.size() >= 0) res_helper.reserve(helper.size());
        for (auto it = std::begin(container); it != std::end(container); ++it) {
            res_helper.insert(it, helper.invoke(fn, it));
        }
        return res;
    }
}

template <class Container>
inline auto sorted(Container&& container) {
    _internal::container_helper helper(container);
    using T = typename decltype(helper)::iterated_type;
    using ResultType = std::vector<T>;

    if constexpr (std::is_base_of_v<ResultType, Container /* rvalue */> && !std::is_const_v<Container>) {
        std::sort(container.begin(), container.end());
        return container;
    } else {
        ResultType res(std::begin(container), std::end(container));
        std::sort(res.begin(), res.end());
        return res;
    }
}

template <class Container, typename KeyFunction>
inline auto sorted(Container&& container, KeyFunction&& fn) {
    _internal::container_helper helper(container);
    using T = typename decltype(helper)::iterated_type;
    using U = typename decltype(helper)::template invoke_result_t<KeyFunction>;
    using P = std::pair<decltype(std::begin(container)), U>;
    using ResultType = std::vector<T>;

    std::vector<P> temp;
    if (helper.size() >= 0) temp.reserve(helper.size());
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        temp.emplace_back(it, helper.invoke(fn, it));
    }
    std::sort(temp.begin(), temp.end(), [](const P& lhs, const P& rhs) { return lhs.second < rhs.second; });

    if constexpr (std::is_base_of_v<ResultType, Container /* rvalue */> && !std::is_const_v<Container>) {
        for (std::size_t i = 0; i < container.size(); ++i) container[i] = *temp[i].first;
        return container;
    } else {
        ResultType res;
        res.reserve(temp.size());
        for (auto&& [it, u] : temp) res.push_back(*it);
        return res;
    }
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
        [&vals](const T& val) -> _internal::signed_size_t {
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
        [&keys, &temp, &i]() -> _internal::signed_size_t {
            return std::lower_bound(keys.begin(), keys.end(), temp[i++]) - keys.begin();
        });
}

template <class Container>
inline auto inversed(Container&& container) {
    _internal::container_helper helper(container);
    static_assert(decltype(helper)::subscriptable, "given container is not subscriptable");
    using K = typename decltype(helper)::key_type;
    using V = typename decltype(helper)::value_type;

    using ResultType = typename _internal::container_selector<V>::template map<K>;
    ResultType res;
    for (auto it = std::begin(container); it != std::end(container); ++it) {
        res[helper.get_value(it)] = helper.get_key(it);
    }
    return res;
}

}  // namespace mihatsu
