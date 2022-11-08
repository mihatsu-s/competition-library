#pragma once

#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#endif

namespace mihatsu {

namespace _internal {
template <std::nullptr_t = nullptr>
struct Init {
    Init() {
#ifndef DEBUG
        std::cin.tie(0);
        std::cout.tie(0);
        std::ios::sync_with_stdio(0);
#endif
        std::cout.precision(16);
    }
};
inline Init init;
}  // namespace _internal

template <typename T>
using unpriority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename T, typename U>
inline T& chmax(T& x, U&& y) {
    return (x < y) ? (x = std::forward<U>(y)) : x;
}
template <typename T, typename U>
inline T& chmin(T& x, U&& y) {
    return (y < x) ? (x = std::forward<U>(y)) : x;
}

}  // namespace mihatsu

#include "../container/MultiDimensionalVector.hpp"
#include "../util/b2e.hpp"
#include "../util/debug_print.hpp"
#include "../util/tuple.hpp"
#include "../util/vector.hpp"
#include "../util/with_input.hpp"

template <typename T>
using V = std::vector<T>;
template <typename T>
using VV = V<V<T>>;
template <typename T>
using VVV = V<VV<T>>;
using ll = long long;
using ull = unsigned long long;
using pll = std::pair<ll, ll>;
using tll = std::tuple<ll, ll, ll>;
using vll = std::vector<ll>;
using dd = long double;
using lli = mihatsu::with_input<ll>;
using ulli = mihatsu::with_input<ull>;
using plli = std::pair<lli, lli>;
using tlli = std::tuple<lli, lli, lli>;
using vlli = std::vector<lli>;
using chari = mihatsu::with_input<char>;
using inti = mihatsu::with_input<int>;
using ddi = mihatsu::with_input<dd>;
using stringi = mihatsu::with_input<std::string>;
constexpr inline ll inf = std::numeric_limits<ll>::max() / 4;
constexpr inline char lf = '\n';
const inline long double pi = acos(-1.0L);

#include "../util/macro.h"
#define rep(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP, __VA_ARGS__)
#define _MIHATSU_REP2(i, n) for (long long i = 0ll, __##i##__end = (n); i < __##i##__end; ++i)
#define _MIHATSU_REP3(i, a, b) for (long long i = (a), __##i##__end = (b); i < __##i##__end; ++i)
#define _MIHATSU_REP4(i, a, b, c) for (long long i = (a), __##i##__end = (b), __##i##__step = (c); __##i##__step > 0ll ? i < __##i##__end : i > __##i##__end; i += __##i##__step)
#define rep_rev(...) MIHATSU_NARGS_SELECTOR(_MIHATSU_REP_REV, __VA_ARGS__)
#define _MIHATSU_REP_REV2(i, n) for (long long i = static_cast<long long>(n) - 1; i >= 0; --i)
#define _MIHATSU_REP_REV3(i, a, b) for (long long i = static_cast<long long>(b) - 1, __##i##__end = (a); i >= __##i##__end; --i)
#define itr(...) _MIHATSU_ITR(MIHATSU_REWIND(__VA_ARGS__))
#define _MIHATSU_ITR(...) _MIHATSU_PRIMITIVE_ITR(__VA_ARGS__)
#define _MIHATSU_PRIMITIVE_ITR(v, ...) for (auto&& __VA_ARGS__ : (v))
#define itr_rev(...) _MIHATSU_ITR_REV(__COUNTER__, MIHATSU_REWIND(__VA_ARGS__))
#define _MIHATSU_ITR_REV(...) _MIHATSU_PRIMITIVE_ITR_REV(__VA_ARGS__)
#define _MIHATSU_PRIMITIVE_ITR_REV(id, v, ...)                                                      \
    if (auto&& __itr_rev_##id##__range = (v); true)                                                 \
        if (auto __itr_rev_##id##__begin = std::rbegin(__itr_rev_##id##__range); true)              \
            if (auto __itr_rev_##id##__end = std::rend(__itr_rev_##id##__range); true)              \
                for (; __itr_rev_##id##__begin != __itr_rev_##id##__end; ++__itr_rev_##id##__begin) \
                    if (auto&& __VA_ARGS__ = *__itr_rev_##id##__begin; true)

using namespace std;
using namespace mihatsu;
