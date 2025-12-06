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

template <bool, typename T = void>
struct enable_if {
};
template <typename T>
struct enable_if<true, T> {
    using type = T;
};

template <typename T, T V>
struct integral_constant {
    static constexpr T value = V;
    using value_type         = T;
    using type               = integral_constant<T, V>;

    constexpr operator value_type() const noexcept { return value; }
};

template <typename _Tp>
struct remove_cv {
    using type = _Tp;
};
template <typename _Tp>
struct remove_cv<const _Tp> {
    using type = _Tp;
};

template <typename _Tp>
struct remove_cv<volatile _Tp> {
    using type = _Tp;
};

template <typename _Tp>
struct remove_cv<const volatile _Tp> {
    using type = _Tp;
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <typename>
struct __is_integral : public false_type{};
template <>
struct __is_integral<bool> : public true_type{};
template <>
struct __is_integral<char> : public true_type{};
template <>
struct __is_integral<short> : public true_type{};
template <>
struct __is_integral<int> : public true_type{};
template <>
struct __is_integral<long> : public true_type{};
template <>
struct __is_integral<unsigned short> : public true_type{};
template <>
struct __is_integral<unsigned int> : public true_type{};
template <>
struct __is_integral<unsigned long> : public true_type{};
template <>
struct __is_integral<signed char> : public true_type{};
template <>
struct __is_integral<unsigned char> : public true_type{};
template <>
struct __is_integral<wchar_t> : public true_type{};

template <typename>
struct __is_bool : public false_type {};
template <>
struct __is_bool<bool> : public true_type {};

template <typename T>
struct is_integral : public __is_integral<typename remove_cv<T>::type> {};
template <typename T>
struct is_bool : public __is_bool<typename remove_cv<T>::type> {};

template <bool C, typename T = void>
using enable_if_t = typename enable_if<C, T>::type;


} // namespace msd
