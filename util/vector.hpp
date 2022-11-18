#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

// -------- input --------

namespace std {
template <class T, class Allocator>
inline istream& operator>>(istream& is, vector<T, Allocator>& vector) {
    for (auto&& p : vector) cin >> p;
    return is;
}
}  // namespace std

// -------- output --------

namespace mihatsu::_internal {

template <class T, class Allocator>
struct VectorPrinter {
    std::ostream& os;
    using Vector = std::vector<T, Allocator>;
    const Vector* ptr_l = nullptr;
    std::unique_ptr<const Vector> ptr_r;

    VectorPrinter(std::ostream& os, const Vector& vector)
        : os(os), ptr_l(&vector) {}
    VectorPrinter(std::ostream& os, Vector&& vector)
        : os(os), ptr_r(std::make_unique<Vector>(std::forward<Vector>(vector))) {}

    inline const Vector& get_vector() {
        return (this->ptr_l != nullptr) ? *(this->ptr_l) : *(this->ptr_r);
    }

    template <typename Delimiter>
    std::ostream& operator<<(const Delimiter& delimiter) {
        bool is_first = true;
        for (auto&& v : get_vector()) {
            if (!is_first) os << delimiter;
            is_first = false;
            os << v;
        }
        return os;
    }
};

}  // namespace mihatsu::_internal

namespace std {

template <class T, class Allocator>
inline auto operator<<(ostream& os, const vector<T, Allocator>& vector) {
    return mihatsu::_internal::VectorPrinter(os, vector);
}
template <class T, class Allocator>
inline auto operator<<(ostream& os, vector<T, Allocator>&& vector) {
    return mihatsu::_internal::VectorPrinter(os, std::forward<std::vector<T, Allocator>>(vector));
}

}  // namespace std
