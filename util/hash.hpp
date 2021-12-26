size_t __hash_sequence() {
    return 0;
}

template<typename Head, typename... Tail>
size_t __hash_sequence(const Head& head, const Tail& ...tail) {
    return std::hash<Head>()(head) ^ __hash_sequence(tail...);
}

namespace std {

template <typename T1, typename T2>
class hash<std::pair<T1, T2>> {
   public:
    size_t operator()(const std::pair<T1, T2>& x) const {
        return __hash_sequence(x.first, x.second);
    }
};

template <typename... T>
class hash<std::tuple<T...>> {
   public:
    size_t operator()(const std::tuple<T...>& x) const {
        return std::apply(__hash_sequence<T...>, x);
    }
};

}  // namespace std
