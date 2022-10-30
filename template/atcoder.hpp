#include "template.hpp"

#include <atcoder/all>

template <int m, void* _>
ostream& operator<<(ostream& s, const atcoder::static_modint<m, _>& x) {
    return s << x.val();
}
template <int id>
ostream& operator<<(ostream& s, const atcoder::dynamic_modint<id>& x) {
    return s << x.val();
}

using namespace atcoder;
