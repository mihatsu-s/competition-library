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

    template <typename Invocable, std::enable_if_t<std::is_invocable_v<Invocable, T>>* = nullptr>
    auto operator<<(Invocable f) {
        using U = std::invoke_result_t<Invocable, T>;
        const Vector& vector = get_vector();
        std::vector<U, std::allocator<U>> new_vector;
        new_vector.reserve(vector.size());
        for (auto&& v : vector) new_vector.push_back(std::invoke(f, v));
        return VectorPrinter<U, std::allocator<U>>(os, std::move(new_vector));
    }

    template <typename Delimiter, std::enable_if_t<!std::is_invocable_v<Delimiter, T>>* = nullptr>
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
