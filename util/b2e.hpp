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

    // first call
    static inline auto begin(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return begin();
    }
    static inline auto end(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return end();
    }
    static inline auto rbegin(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return rbegin();
    }
    static inline auto rend(T&& val) {
        ptr = std::make_unique<T>(std::forward<T>(val));
        return rend();
    }

    // second call
    static inline auto begin() {
        return std::begin(*ptr);
    }
    static inline auto end() {
        return std::end(*ptr);
    }
    static inline auto rbegin() {
        return std::rbegin(*ptr);
    }
    static inline auto rend() {
        return std::rend(*ptr);
    }

    // not called (written only for type consistency)
    static inline auto begin(T& val) {
        return std::begin(val);
    }
    static inline auto end(T& val) {
        return std::end(val);
    }
    static inline auto rbegin(T& val) {
        return std::rbegin(val);
    }
    static inline auto rend(T& val) {
        return std::rend(val);
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
#define _MIHATSU_B2E_EXPR(name, val, impl, T, id)                                                         \
    (std::is_reference_v<decltype((val))> ? std::name(val)                                                \
     : impl<T, id>::is_first()            ? static_cast<decltype(std::name(val))>(impl<T, id>::name(val)) \
                                          : static_cast<decltype(std::name(val))>(impl<T, id>::name()))

}  // namespace mihatsu::_internal
