#pragma once

#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <cmath>
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

#include "../container/MultiDimensionalVector.hpp"
#include "../util/b2e.hpp"
#include "../util/container.hpp"
#include "../util/debug_print.hpp"
#include "../util/general.hpp"
#include "../util/lambda_shorthand.hpp"
#include "../util/macro.h"
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
template <typename T>
using unpriority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
constexpr inline ll inf = std::numeric_limits<ll>::max() / 4;
constexpr inline char lf = '\n';
constexpr inline long double pi = 3.1415926535897932384626L;
constexpr inline auto X = mihatsu::lambda_shorthand::make_variable<0>();
constexpr inline auto Y = mihatsu::lambda_shorthand::make_variable<1>();

namespace mihatsu::_internal {
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
}  // namespace mihatsu::_internal

using namespace std;
using namespace mihatsu;

// -------- 以下, 仮コード --------

namespace std {
template <typename T, enable_if_t<mihatsu::_internal::is_pairlike_v<T>>* = nullptr>
ostream& operator<<(ostream& os, const T& pair) {
    return os << pair.first << " " << pair.second;
}
}  // namespace std

namespace mihatsu::_internal {
template <bool is_const>
struct container_helper<stringi, is_const> : container_helper<std::string, is_const> {
    using container_helper<std::string, is_const>::container_helper;
};
}  // namespace mihatsu::_internal
