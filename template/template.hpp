#ifndef MIHATSU_TEMPLATE_HPP
#define MIHATSU_TEMPLATE_HPP

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
Init init;
}  // namespace _internal

template <typename T>
using unpriority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename T, typename U>
inline T& chmax(T& x, const U& y) {
    return (x < y) ? (x = y) : x;
}
template <typename T, typename U>
inline T& chmax(T& x, U&& y) {
    return (x < y) ? (x = y) : x;
}
template <typename T, typename U>
inline T& chmin(T& x, const U& y) {
    return (x > y) ? (x = y) : x;
}
template <typename T, typename U>
inline T& chmin(T& x, U&& y) {
    return (x > y) ? (x = y) : x;
}

}  // namespace mihatsu

#include "../util/debug_print.hpp"
#include "../util/input.hpp"
#include "../util/tuple.hpp"

template <typename T>
using V = std::vector<T>;
template <typename T>
using VV = V<V<T>>;
template <typename T>
using VVV = V<VV<T>>;
template <typename T>
using VVVV = V<VVV<T>>;
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

#include "../macro/util.h"
#define rep(...) NARGS_SELECTOR(__REP, __VA_ARGS__)
#define __REP2(i, n) for (long long i = 0ll, __##i##__end = (n); i < __##i##__end; ++i)
#define __REP3(i, a, b) for (long long i = (a), __##i##__end = (b); i < __##i##__end; ++i)
#define __REP4(i, a, b, c) for (long long i = (a), __##i##__end = (b), __##i##__step = (c); __##i##__step > 0ll ? i < __##i##__end : i > __##i##__end; i += __##i##__step)
#define itr(...) __ITR(REWIND(__VA_ARGS__))
#define __ITR(...) __PRIMITIVE_ITR(__VA_ARGS__)
#define __PRIMITIVE_ITR(v, ...) for (auto&& __VA_ARGS__ : (v))
#define b2e(x) (x).begin(), (x).end()
#define e2b(x) (x).rbegin(), (x).rend()

using namespace std;
using namespace mihatsu;

#endif
