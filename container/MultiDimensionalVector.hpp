#pragma once

#include <tuple>
#include <utility>
#include <vector>
#include "../util/meta.hpp"

namespace mihatsu {

namespace _internal {

template <typename T, std::size_t N>
struct nested_vector {
    using type = std::vector<typename nested_vector<T, N - 1>::type>;
};
template <typename T>
struct nested_vector<T, 0> {
    using type = T;
};
template <typename T, std::size_t N>
using nested_vector_t = typename nested_vector<T, N>::type;

}  // namespace _internal

template <typename T, std::size_t dimension>
class MultiDimensionalVector : public _internal::nested_vector_t<T, dimension> {
    static_assert(dimension > 0, "dimension of MultiDimensionalVector must be greater than 0");
    using Vector = _internal::nested_vector_t<T, dimension>;
    using Size = _internal::arraylike_tuple_t<std::size_t, dimension>;

    template <std::size_t i>
    constexpr void init(_internal::nested_vector_t<T, dimension - i>& vector, const Size& size) {
        vector.resize(std::get<i>(size));
        if constexpr (i + 1 < dimension) {
            for (auto&& v : vector) init<i + 1>(v, size);
        }
    }
    template <std::size_t i>
    constexpr void init(_internal::nested_vector_t<T, dimension - i>& vector, const Size& size, const T& initializer) {
        const std::size_t n = std::get<i>(size);
        if constexpr (i + 1 < dimension) {
            vector.resize(n);
            for (auto&& v : vector) init<i + 1>(v, size, initializer);
        } else {
            vector.reserve(n);
            for (std::size_t p = 0; p < n; ++p) vector.emplace_back(initializer);
        }
    }

   public:
    using Vector::Vector;
    constexpr inline MultiDimensionalVector(const Vector& vector)
        : Vector{vector} {}
    constexpr inline MultiDimensionalVector(Vector&& vector)
        : Vector{std::forward<Vector>(vector)} {}

    constexpr inline MultiDimensionalVector(const Size& size) {
        init<0>(*this, size);
    };
    constexpr inline MultiDimensionalVector(const Size& size, const T& initializer) {
        init<0>(*this, size, initializer);
    };
};
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 0>&, const T&) -> MultiDimensionalVector<T, 0>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 1>&, const T&) -> MultiDimensionalVector<T, 1>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 2>&, const T&) -> MultiDimensionalVector<T, 2>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 3>&, const T&) -> MultiDimensionalVector<T, 3>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 4>&, const T&) -> MultiDimensionalVector<T, 4>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 5>&, const T&) -> MultiDimensionalVector<T, 5>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 6>&, const T&) -> MultiDimensionalVector<T, 6>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 7>&, const T&) -> MultiDimensionalVector<T, 7>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 8>&, const T&) -> MultiDimensionalVector<T, 8>;
template <typename T>
MultiDimensionalVector(const _internal::arraylike_tuple_t<std::size_t, 9>&, const T&) -> MultiDimensionalVector<T, 9>;

}  // namespace mihatsu
