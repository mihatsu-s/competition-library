#pragma once

#ifndef MIHATSU_B2E_STORE_SIZE
#define MIHATSU_B2E_STORE_SIZE 16
#endif
static_assert(MIHATSU_B2E_STORE_SIZE > 0, "MIHATSU_B2E_STORE_SIZE must be greater than 0");

#include <iterator>
#include <memory>
#include <type_traits>

namespace mihatsu::_internal {

template <typename T, int id>
struct b2e_impl {
   private:
    static std::unique_ptr<T> ptr;
    static int counter;

   public:
    static inline bool is_first() noexcept {
        return ++counter % 2 == 1;
    }

    using begin_type = decltype(std::begin(std::declval<T>()));
    using end_type = decltype(std::end(std::declval<T>()));
    using rbegin_type = decltype(std::rbegin(std::declval<T>()));
    using rend_type = decltype(std::rend(std::declval<T>()));

    // for lvalue
    static inline begin_type begin(T& val) {
        return std::begin(val);
    }
    static inline end_type end(T& val) {
        return std::end(val);
    }
    static inline rbegin_type rbegin(T& val) {
        return std::rbegin(val);
    }
    static inline rend_type rend(T& val) {
        return std::rend(val);
    }

    // for rvalue (first call)
    static inline begin_type begin(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return begin();
    }
    static inline end_type end(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return end();
    }
    static inline rbegin_type rbegin(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return rbegin();
    }
    static inline rend_type rend(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return rend();
    }

    // for rvalue (second call)
    static inline begin_type begin() {
        return std::begin(*ptr);
    }
    static inline end_type end() {
        return std::end(*ptr);
    }
    static inline rbegin_type rbegin() {
        return std::rbegin(*ptr);
    }
    static inline rend_type rend() {
        return std::rend(*ptr);
    }
};
template <typename T, int id>
inline int b2e_impl<T, id>::counter = 0;
template <typename T, int id>
inline std::unique_ptr<T> b2e_impl<T, id>::ptr;

#define b2e(val) _MIHATSU_B2E(val, begin, end)
#define e2b(val) _MIHATSU_B2E(val, rbegin, rend)
#define _MIHATSU_B2E(val, begin_name, end_name) _MIHATSU_B2E_IMPL(val, begin_name, end_name, mihatsu::_internal::b2e_impl, std::remove_reference_t<decltype(val)>, __COUNTER__ % MIHATSU_B2E_STORE_SIZE)
#define _MIHATSU_B2E_IMPL(val, begin_name, end_name, impl, T, id) _MIHATSU_B2E_EXPR(begin_name, val, impl, T, id), _MIHATSU_B2E_EXPR(end_name, val, impl, T, id)
#define _MIHATSU_B2E_EXPR(name, val, impl, T, id)                  \
    (std::is_reference_v<decltype((val))> ? impl<T, id>::name(val) \
     : impl<T, id>::is_first()            ? impl<T, id>::name(val) \
                                          : impl<T, id>::name())

}  // namespace mihatsu::_internal
