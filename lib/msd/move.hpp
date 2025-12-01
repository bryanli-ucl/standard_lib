#pragma once

namespace msd {

// remove reference
template <typename T>
struct remove_reference {
    using type = T;
};
template <typename T>
struct remove_reference<T&> {
    using type = T;
};
template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

// forward
template <typename T>
constexpr T&& forward(typename remove_reference<T>::type& val) {
    return static_cast<T&&>(val);
}
template <typename T>
constexpr T&& forward(typename remove_reference<T>::type&& val) {
    return static_cast<T&&>(val);
}

// move
template <typename T>
constexpr typename remove_reference<T>::type&& move(T&& val) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(val);
}

} // namespace msd
