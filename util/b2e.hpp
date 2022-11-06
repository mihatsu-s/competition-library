#pragma once

#include <iterator>
#include <memory>
#include <type_traits>

namespace mihatsu::_internal {

template <typename T>
struct b2e_impl_lvalue {
    static T* ptr;
    static inline auto begin(T& val) {
        ptr = &val;
        return std::begin(*ptr);
    }
    static inline auto end() {
        return std::end(*ptr);
    }
    static inline auto rbegin(T& val) {
        ptr = &val;
        return std::rbegin(*ptr);
    }
    static inline auto rend() {
        return std::rend(*ptr);
    }
};
template <typename T>
inline T* b2e_impl_lvalue<T>::ptr;

template <typename T>
struct b2e_impl_rvalue {
    static std::unique_ptr<T> ptr;
    static inline auto begin(T&& val) {
        ptr = std::make_unique<T>(val);  // call move constructor
        return std::begin(*ptr);
    }
    static inline auto end() {
        return std::end(*ptr);
        // *ptr will be released on next begin() call...
    }
    static inline auto rbegin(T&& val) {
        ptr = std::make_unique<T>(val);
        return std::rbegin(*ptr);
    }
    static inline auto rend() {
        return std::rend(*ptr);
    }
};
template <typename T>
inline std::unique_ptr<T> b2e_impl_rvalue<T>::ptr;

template <typename T>
struct b2e_selector {
    using impl = b2e_impl_rvalue<T>;
};
template <typename T>
struct b2e_selector<T&> {
    using impl = b2e_impl_lvalue<T>;
};

#define b2e(val) (mihatsu::_internal::b2e_selector<decltype((val))>::impl::begin(val)), (mihatsu::_internal::b2e_selector<decltype((val))>::impl::end())
#define e2b(val) (mihatsu::_internal::b2e_selector<decltype((val))>::impl::rbegin(val)), (mihatsu::_internal::b2e_selector<decltype((val))>::impl::rend())

}  // namespace mihatsu::_internal
