#include <cassert>
#include <utility>
#include <vector>

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

    template <std::size_t i>
    constexpr inline void init(_internal::nested_vector_t<T, dimension - i>& vector, const std::initializer_list<std::size_t>& size) {
        vector.resize((size.begin())[i]);
        if constexpr (i + 1 < dimension) {
            for (auto&& v : vector) init<i + 1>(v, size);
        }
    }
    template <std::size_t i>
    constexpr inline void init(_internal::nested_vector_t<T, dimension - i>& vector, const std::initializer_list<std::size_t>& size, const T& initializer) {
        const std::size_t n = (size.begin())[i];
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

    constexpr inline MultiDimensionalVector(const std::initializer_list<std::size_t>& size) {
        assert(size.size() == dimension);
        init<0>(*this, size);
    };
    constexpr inline MultiDimensionalVector(const std::initializer_list<std::size_t>& size, const T& initializer) {
        assert(size.size() == dimension);
        init<0>(*this, size, initializer);
    };
};

}  // namespace mihatsu
