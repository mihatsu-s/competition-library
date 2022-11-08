#pragma once

#include <iostream>
#include <vector>

namespace std {

template <class T, class Allocator>
inline istream& operator>>(istream& is, vector<T, Allocator>& vector) {
    for (auto&& p : vector) cin >> p;
    return is;
}

}  // namespace std
