#include <concepts>
#include <cstddef>
#include <iterator>

template <typename C>
concept Iterable = requires(C c) {
    std::begin(c);
    std::end(c);
};

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template <typename T>
concept Divisible = requires(T a, std::size_t n) {
    { a / n } -> std::same_as<T>;
};

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::same_as<bool>;
};

template <Iterable C>
    requires Addable<typename C::value_type>
auto sum(const C &container) {
    using T = typename C::value_type;
    T result{};

    for (const auto &value : container)
        result += value;

    return result;
};

template <Iterable C>
    requires Addable<typename C::value_type> && Divisible<typename C::value_type>
auto mean(const C &container) {
    auto total = sum(container);
    auto n = static_cast<std::size_t>(std::distance(std::begin(container), std::end(container)));
    return total / n;
};

template <Iterable C>
    requires Addable<typename C::value_type> && Divisible<typename C::value_type>
auto variance(const C &container) {
    using T = typename C::value_type;
    auto m = mean(container);
    T acc{};
    std::size_t n = 0;

    for (const auto &value : container) {
        auto diff = value - m;
        acc = acc + diff * diff;
        ++n;
    }

    return acc / n;
};