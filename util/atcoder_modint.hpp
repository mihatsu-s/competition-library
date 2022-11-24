#pragma once

#include <atcoder/modint>
#include <iostream>

namespace atcoder {

template <int m, void* _>
inline std::istream& operator>>(std::istream& s, static_modint<m, _>& x) {
    long long v;
    s >> v;
    x = v;
    return s;
}
template <int id>
inline std::istream& operator>>(std::istream& s, dynamic_modint<id>& x) {
    long long v;
    s >> v;
    x = v;
    return s;
}

template <int m, void* _>
inline std::ostream& operator<<(std::ostream& s, const static_modint<m, _>& x) {
    return s << x.val();
}
template <int id>
inline std::ostream& operator<<(std::ostream& s, const dynamic_modint<id>& x) {
    return s << x.val();
}

template <int m, void* _>
inline bool operator<(const static_modint<m, _>& lhs, const static_modint<m, _>& rhs) {
    return lhs.val() < rhs.val();
}
template <int id>
inline bool operator<(const dynamic_modint<id>& lhs, const dynamic_modint<id>& rhs) {
    return lhs.val() < rhs.val();
}

}  // namespace atcoder

namespace std {

template <int m, void* _>
class hash<atcoder::static_modint<m, _>> {
   public:
    inline size_t operator()(const atcoder::static_modint<m, _>& x) const {
        return x.val();
    }
};
template <int id>
class hash<atcoder::dynamic_modint<id>> {
   public:
    inline size_t operator()(const atcoder::dynamic_modint<id>& x) const {
        return x.val();
    }
};

}  // namespace std
