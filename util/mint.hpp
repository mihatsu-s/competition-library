#ifndef MIHATSU_UTIL_MINT_HPP
#define MIHATSU_UTIL_MINT_HPP 1

#include <atcoder/modint>
#include <iostream>

namespace std {

template <int m, void* _>
std::istream& operator>>(std::istream& s, atcoder::static_modint<m, _>& x) {
    long long v;
    s >> v;
    x = v;
    return s;
}
template <int m, void* _>
std::ostream& operator<<(std::ostream& s, const atcoder::static_modint<m, _>& x) {
    return s << x.val();
}

template <int id>
std::istream& operator>>(std::istream& s, atcoder::dynamic_modint<id>& x) {
    long long v;
    s >> v;
    x = v;
    return s;
}
template <int id>
std::ostream& operator<<(std::ostream& s, const atcoder::dynamic_modint<id>& x) {
    return s << x.val();
}

}  // namespace std

#endif
