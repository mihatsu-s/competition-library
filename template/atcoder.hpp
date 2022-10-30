#ifndef MIHATSU_TEMPLATE_ATCODER_HPP
#define MIHATSU_TEMPLATE_ATCODER_HPP 1

#include "template.hpp"

#include <atcoder/all>

template <int m, void* _>
std::ostream& operator<<(std::ostream& s, const atcoder::static_modint<m, _>& x) {
    return s << x.val();
}
template <int id>
std::ostream& operator<<(std::ostream& s, const atcoder::dynamic_modint<id>& x) {
    return s << x.val();
}

using namespace atcoder;

#endif
