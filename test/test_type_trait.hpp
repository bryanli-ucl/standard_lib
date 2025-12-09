// msd_type_traits_unity_test.c
#pragma once

#include <stdio.h>
#include <unity.h>

// ============ C++ 部分 ============
#ifdef __cplusplus

#include <type_traits>
namespace msd_type_traits_unity_test {
namespace msd_test {

// 测试辅助类

// 可移动类
class MovableClass {
    public:
    MovableClass() = default;
    MovableClass(MovableClass&&) noexcept {}
    MovableClass& operator=(MovableClass&&) noexcept { return *this; }
};

// 可能抛出异常的移动类
class MovableClassThrows {
    public:
    MovableClassThrows() = default;
    MovableClassThrows(MovableClassThrows&&) {} // 没有noexcept
    MovableClassThrows& operator=(MovableClassThrows&&) { return *this; }
};

// 不可移动类
class NonMovableClass {
    public:
    NonMovableClass()                                  = default;
    NonMovableClass(const NonMovableClass&)            = delete;
    NonMovableClass(NonMovableClass&&)                 = delete;
    NonMovableClass& operator=(const NonMovableClass&) = delete;
    NonMovableClass& operator=(NonMovableClass&&)      = delete;
};

} // namespace msd_test

#endif // __cplusplus

// ============ Unity 测试函数 ============

#ifdef __cplusplus
extern "C" {
#endif

// 测试2: remove_cv
void test_remove_cv(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试无CV限定符
    TEST_ASSERT_TRUE((is_same<remove_cv_t<int>, int>::value));

    // 测试const
    TEST_ASSERT_TRUE((is_same<remove_cv_t<const int>, int>::value));

    // 测试volatile
    TEST_ASSERT_TRUE((is_same<remove_cv_t<volatile int>, int>::value));

    // 测试const volatile
    TEST_ASSERT_TRUE((is_same<remove_cv_t<const volatile int>, int>::value));

    // 测试复合类型
    TEST_ASSERT_TRUE((is_same<remove_cv_t<const int* const>, const int*>::value));
    TEST_ASSERT_TRUE((is_same<remove_cv_t<const volatile double&>, const volatile double&>::value));

    printf("✓ test_remove_cv passed\n");
#endif
}

// 测试3: void_t
void test_void_t(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试void_t可以接受任意类型
    TEST_ASSERT_TRUE((is_same<void_t<>, void>::value));
    TEST_ASSERT_TRUE((is_same<void_t<int>, void>::value));
    TEST_ASSERT_TRUE((is_same<void_t<int, double, char>, void>::value));

    // // 测试在SFINAE中使用
    // template <typename T, typename = void_t<>>
    // struct has_type_member : false_type {};

    // template <typename T>
    // struct has_type_member<T, void_t<typename T::type>> : true_type {};

    // struct WithType {
    //     using type = int;
    // };
    // struct WithoutType {};

    // TEST_ASSERT_TRUE(has_type_member<WithType>::value);
    // TEST_ASSERT_FALSE(has_type_member<WithoutType>::value);

    printf("✓ test_void_t passed\n");
#endif
}

// 测试4: add_rvalue_reference
void test_add_rvalue_reference(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试基本类型
    TEST_ASSERT_TRUE((is_same<typename add_rvalue_reference<int>::type, int&&>::value));
    TEST_ASSERT_TRUE((is_same<typename add_rvalue_reference<double>::type, double&&>::value));

    // 测试引用类型（引用折叠规则）
    TEST_ASSERT_TRUE((is_same<typename add_rvalue_reference<int&>::type, int&>::value));
    TEST_ASSERT_TRUE((is_same<typename add_rvalue_reference<int&&>::type, int&&>::value));

    // 测试const/volatile
    TEST_ASSERT_TRUE((is_same<typename add_rvalue_reference<const int>::type, const int&&>::value));

    printf("✓ test_add_rvalue_reference passed\n");
#endif
}

// 测试5: enable_if
void test_enable_if(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试enable_if条件为true
    TEST_ASSERT_TRUE((is_same<enable_if_t<true, int>, int>::value));
    TEST_ASSERT_TRUE((is_same<enable_if_t<true, double>, double>::value));

    // // 测试enable_if条件为false（应该没有type成员）
    // struct NoTypeCheck {
    //     template <bool B, typename T = void>
    //     static constexpr bool has_type = true;
    // };

    // template <>
    // struct NoTypeCheck::has_type<false, void> {
    //     static constexpr bool value = false;
    // };

    // // 使用enable_if的实际场景
    // template <typename T, typename = enable_if_t<is_integral<T>::value>>
    // struct OnlyIntegral {
    //     static constexpr bool value = true;
    // };

    // template <typename T, typename>
    // struct OnlyIntegral<T, void> {
    //     static constexpr bool value = false;
    // };

    // TEST_ASSERT_TRUE(OnlyIntegral<int>::value);
    // TEST_ASSERT_FALSE(OnlyIntegral<double>::value);

    printf("✓ test_enable_if passed\n");
#endif
}

// 测试6: conditional
void test_conditional(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试条件为true
    TEST_ASSERT_TRUE((is_same<conditional_t<true, int, double>, int>::value));
    TEST_ASSERT_TRUE((is_same<conditional_t<true, const char*, int>, const char*>::value));

    // 测试条件为false
    TEST_ASSERT_TRUE((is_same<conditional_t<false, int, double>, double>::value));
    TEST_ASSERT_TRUE((is_same<conditional_t<false, char, long>, long>::value));

    // 测试嵌套conditional
    using type1 = conditional_t<true, conditional_t<false, float, double>, int>;
    TEST_ASSERT_TRUE((is_same<type1, double>::value));

    using type2 = conditional_t<false, float, conditional_t<true, char, int>>;
    TEST_ASSERT_TRUE((is_same<type2, char>::value));

    printf("✓ test_conditional passed\n");
#endif
}

// 测试7: constant和true_type/false_type
void test_constant_and_types(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试constant
    constant<int, 42> int_constant;
    TEST_ASSERT_EQUAL_INT(42, int_constant.value);
    TEST_ASSERT_EQUAL_INT(42, int_constant());
    TEST_ASSERT_EQUAL_INT(42, static_cast<int>(int_constant));

    constant<bool, true> true_constant;
    TEST_ASSERT_TRUE(true_constant.value);
    TEST_ASSERT_TRUE(true_constant());

    // 测试true_type
    TEST_ASSERT_TRUE(true_type::value);
    TEST_ASSERT_TRUE(true_type()());
    TEST_ASSERT_TRUE(static_cast<bool>(true_type()));

    // 测试false_type
    TEST_ASSERT_FALSE(false_type::value);
    TEST_ASSERT_FALSE(false_type()());
    TEST_ASSERT_FALSE(static_cast<bool>(false_type()));

    // 测试自定义constant
    using my_const = constant<long, 1000L>;
    TEST_ASSERT_EQUAL_UINT64(1000L, my_const::value);
    TEST_ASSERT_TRUE((is_same<my_const::val_t, long>::value));

    printf("✓ test_constant_and_types passed\n");
#endif
}

// 测试8: conjunction (逻辑与)
void test_conjunction(void) {
#ifdef __cplusplus
    using namespace msd;

    // 空参数列表默认为true
    TEST_ASSERT_TRUE(conjunction<>::value);

    // 单个参数
    TEST_ASSERT_TRUE(conjunction<true_type>::value);
    TEST_ASSERT_FALSE(conjunction<false_type>::value);

    // 多个参数，全部为true
    TEST_ASSERT_TRUE((conjunction<true_type, true_type, true_type>::value));

    // 多个参数，包含false
    TEST_ASSERT_FALSE((conjunction<true_type, false_type, true_type>::value));

    // 第一个为false，短路求值
    TEST_ASSERT_FALSE((conjunction<false_type, true_type>::value));

    // 使用实际类型特性
    TEST_ASSERT_TRUE((conjunction<is_integral<int>, is_integral<long>>::value));
    TEST_ASSERT_FALSE((conjunction<is_integral<int>, is_integral<double>>::value));

    printf("✓ test_conjunction passed\n");
#endif
}

// 测试9: disjunction (逻辑或)
void test_disjunction(void) {
#ifdef __cplusplus
    using namespace msd;

    // 空参数列表默认为false
    TEST_ASSERT_FALSE(disjunction<>::value);

    // 单个参数
    TEST_ASSERT_TRUE(disjunction<true_type>::value);
    TEST_ASSERT_FALSE(disjunction<false_type>::value);

    // 多个参数，至少一个为true
    TEST_ASSERT_TRUE((disjunction<false_type, true_type, false_type>::value));

    // 全部为false
    TEST_ASSERT_FALSE((disjunction<false_type, false_type, false_type>::value));

    // 第一个为true，短路求值
    TEST_ASSERT_TRUE((disjunction<true_type, false_type>::value));

    // 使用实际类型特性
    TEST_ASSERT_TRUE((disjunction<is_integral<double>, is_integral<int>>::value));
    TEST_ASSERT_FALSE((disjunction<is_integral<double>, is_integral<float>>::value));

    printf("✓ test_disjunction passed\n");
#endif
}

// 测试10: negation (逻辑非)
void test_negation(void) {
#ifdef __cplusplus
    using namespace msd;

    // 对true取反
    TEST_ASSERT_FALSE(negation<true_type>::value);

    // 对false取反
    TEST_ASSERT_TRUE(negation<false_type>::value);

    // 对自定义constant取反
    TEST_ASSERT_TRUE((negation<constant<bool, false>>::value));
    TEST_ASSERT_FALSE((negation<constant<bool, true>>::value));

    // 使用实际类型特性
    TEST_ASSERT_TRUE((negation<is_integral<double>>::value));
    TEST_ASSERT_FALSE((negation<is_integral<int>>::value));

    // 嵌套使用
    TEST_ASSERT_TRUE((negation<negation<true_type>>::value));
    TEST_ASSERT_FALSE((negation<negation<false_type>>::value));

    printf("✓ test_negation passed\n");
#endif
}

// 测试11: is_integral
void test_is_integral(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试所有整数类型
    TEST_ASSERT_TRUE(is_integral<bool>::value);
    TEST_ASSERT_TRUE(is_integral<char>::value);
    TEST_ASSERT_TRUE(is_integral<signed char>::value);
    TEST_ASSERT_TRUE(is_integral<unsigned char>::value);
    TEST_ASSERT_TRUE(is_integral<short>::value);
    TEST_ASSERT_TRUE(is_integral<unsigned short>::value);
    TEST_ASSERT_TRUE(is_integral<int>::value);
    TEST_ASSERT_TRUE(is_integral<unsigned int>::value);
    TEST_ASSERT_TRUE(is_integral<long>::value);
    TEST_ASSERT_TRUE(is_integral<unsigned long>::value);
    TEST_ASSERT_TRUE(is_integral<long long>::value);
    TEST_ASSERT_TRUE(is_integral<unsigned long long>::value);

    // 测试宽字符类型
    TEST_ASSERT_TRUE(is_integral<wchar_t>::value);

    // 测试非整数类型
    TEST_ASSERT_FALSE(is_integral<float>::value);
    TEST_ASSERT_FALSE(is_integral<double>::value);
    TEST_ASSERT_FALSE(is_integral<long double>::value);

    // 测试指针类型
    TEST_ASSERT_FALSE(is_integral<int*>::value);
    TEST_ASSERT_FALSE(is_integral<void*>::value);

    // 测试引用类型
    TEST_ASSERT_FALSE(is_integral<int&>::value);

    // 测试const/volatile修饰符
    TEST_ASSERT_TRUE(is_integral<const int>::value);
    TEST_ASSERT_TRUE(is_integral<volatile long>::value);
    TEST_ASSERT_TRUE(is_integral<const volatile char>::value);

    printf("✓ test_is_integral passed\n");
#endif
}

// 测试12: is_bool
void test_is_bool(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试bool类型
    TEST_ASSERT_TRUE(is_bool<bool>::value);
    TEST_ASSERT_TRUE(is_bool<const bool>::value);
    TEST_ASSERT_TRUE(is_bool<volatile bool>::value);
    TEST_ASSERT_TRUE(is_bool<const volatile bool>::value);

    // 测试非bool类型
    TEST_ASSERT_FALSE(is_bool<int>::value);
    TEST_ASSERT_FALSE(is_bool<char>::value);
    TEST_ASSERT_FALSE(is_bool<float>::value);

    // 测试类似bool但不是bool的类型
    TEST_ASSERT_FALSE(is_bool<bool*>::value);
    TEST_ASSERT_FALSE(is_bool<bool&>::value);

    printf("✓ test_is_bool passed\n");
#endif
}

// 测试13: is_floating_point
void test_is_floating_point(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试浮点类型
    TEST_ASSERT_TRUE(is_floating_point<float>::value);
    TEST_ASSERT_TRUE(is_floating_point<double>::value);
    TEST_ASSERT_TRUE(is_floating_point<long double>::value);

    // 测试const/volatile修饰符
    TEST_ASSERT_TRUE(is_floating_point<const float>::value);
    TEST_ASSERT_TRUE(is_floating_point<volatile double>::value);
    TEST_ASSERT_TRUE(is_floating_point<const volatile long double>::value);

    // 测试非浮点类型
    TEST_ASSERT_FALSE(is_floating_point<int>::value);
    TEST_ASSERT_FALSE(is_floating_point<bool>::value);
    TEST_ASSERT_FALSE(is_floating_point<char>::value);

    // 测试指针和引用
    TEST_ASSERT_FALSE(is_floating_point<float*>::value);
    TEST_ASSERT_FALSE(is_floating_point<double&>::value);

    printf("✓ test_is_floating_point passed\n");
#endif
}

// 测试14: is_same
void test_is_same(void) {
#ifdef __cplusplus
    using namespace msd;

    // 相同类型
    TEST_ASSERT_TRUE((is_same<int, int>::value));
    TEST_ASSERT_TRUE((is_same<double, double>::value));
    TEST_ASSERT_TRUE((is_same<const char*, const char*>::value));

    // 不同类型
    TEST_ASSERT_FALSE((is_same<int, long>::value));
    TEST_ASSERT_FALSE((is_same<float, double>::value));
    TEST_ASSERT_FALSE((is_same<char, signed char>::value));

    // 测试const/volatile修饰
    TEST_ASSERT_FALSE((is_same<int, const int>::value));
    TEST_ASSERT_FALSE((is_same<volatile int, int>::value));
    TEST_ASSERT_TRUE((is_same<const int, const int>::value));

    // 测试引用
    TEST_ASSERT_FALSE((is_same<int, int&>::value));
    TEST_ASSERT_FALSE((is_same<int&, int&&>::value));
    TEST_ASSERT_TRUE((is_same<int&, int&>::value));

    // 测试指针
    TEST_ASSERT_FALSE((is_same<int*, const int*>::value));
    TEST_ASSERT_TRUE((is_same<int*, int*>::value));

// 与标准库比较（如果可用）
#ifdef __cpp_lib_type_traits
    TEST_ASSERT_EQUAL(std::is_same<int, int>::value, (is_same<int, int>::value));
    TEST_ASSERT_EQUAL(std::is_same<int, double>::value, (is_same<int, double>::value));
#endif

    printf("✓ test_is_same passed\n");
#endif
}

// 测试15: is_move_constructible
void test_is_move_constructible(void) {
#ifdef __cplusplus
    using namespace msd;
    using msd_test::MovableClass;
    using msd_test::MovableClassThrows;
    using msd_test::NonMovableClass;

    // 测试内置类型（都可移动构造）
    TEST_ASSERT_TRUE(is_move_constructible<int>::value);
    TEST_ASSERT_TRUE(is_move_constructible<double>::value);
    TEST_ASSERT_TRUE(is_move_constructible<int*>::value);

    // 测试自定义可移动类
    TEST_ASSERT_TRUE(is_move_constructible<MovableClass>::value);

    // 测试可能抛出异常的移动类
    TEST_ASSERT_TRUE(is_move_constructible<MovableClassThrows>::value);

    // 测试不可移动类
    TEST_ASSERT_FALSE(is_move_constructible<NonMovableClass>::value);

    // 测试引用类型
    TEST_ASSERT_TRUE(is_move_constructible<int&>::value);
    TEST_ASSERT_TRUE(is_move_constructible<int&&>::value);

    // 测试数组（不可移动构造）
    TEST_ASSERT_FALSE(is_move_constructible<int[5]>::value);

    printf("✓ test_is_move_constructible passed\n");
#endif
}

// 测试16: is_nothrow_move_constructible
void test_is_nothrow_move_constructible(void) {
#ifdef __cplusplus
    using namespace msd;
    using msd_test::MovableClass;
    using msd_test::MovableClassThrows;
    using msd_test::NonMovableClass;

    // 测试内置类型（都无异常）
    TEST_ASSERT_TRUE(is_nothrow_move_constructible<int>::value);
    TEST_ASSERT_TRUE(is_nothrow_move_constructible<double>::value);

    // 测试noexcept移动的类
    TEST_ASSERT_TRUE(is_nothrow_move_constructible<MovableClass>::value);

    // 测试可能抛出异常的移动类
    TEST_ASSERT_FALSE(is_nothrow_move_constructible<MovableClassThrows>::value);

    // 测试不可移动构造的类
    TEST_ASSERT_FALSE(is_nothrow_move_constructible<NonMovableClass>::value);

    printf("✓ test_is_nothrow_move_constructible passed\n");
#endif
}

// 测试17: is_move_assignable
void test_is_move_assignable(void) {
#ifdef __cplusplus
    using namespace msd;
    using msd_test::MovableClass;
    using msd_test::MovableClassThrows;
    using msd_test::NonMovableClass;

    // 测试内置类型
    TEST_ASSERT_TRUE(is_move_assignable<int>::value);
    TEST_ASSERT_TRUE(is_move_assignable<double>::value);
    TEST_ASSERT_TRUE(is_move_assignable<int*>::value);

    // 测试自定义可移动赋值类
    TEST_ASSERT_TRUE(is_move_assignable<MovableClass>::value);

    // 测试可能抛出异常的移动赋值类
    TEST_ASSERT_TRUE(is_move_assignable<MovableClassThrows>::value);

    // 测试不可移动赋值类
    TEST_ASSERT_FALSE(is_move_assignable<NonMovableClass>::value);

    // 测试const类型（不可赋值）
    TEST_ASSERT_FALSE(is_move_assignable<const int>::value);

    // 测试引用类型
    TEST_ASSERT_TRUE(is_move_assignable<int&>::value);
    TEST_ASSERT_TRUE(is_move_assignable<int&&>::value); // 右值引用通常不可赋值

    printf("✓ test_is_move_assignable passed\n");
#endif
}

// 测试18: is_nothrow_move_assignable
void test_is_nothrow_move_assignable(void) {
#ifdef __cplusplus
    using namespace msd;
    using msd_test::MovableClass;
    using msd_test::MovableClassThrows;
    using msd_test::NonMovableClass;

    // 测试内置类型
    TEST_ASSERT_TRUE(is_nothrow_move_assignable<int>::value);
    TEST_ASSERT_TRUE(is_nothrow_move_assignable<double>::value);

    // 测试noexcept移动赋值的类
    TEST_ASSERT_TRUE(is_nothrow_move_assignable<MovableClass>::value);

    // 测试可能抛出异常的移动赋值类
    TEST_ASSERT_FALSE(is_nothrow_move_assignable<MovableClassThrows>::value);

    // 测试不可移动赋值的类
    TEST_ASSERT_FALSE(is_nothrow_move_assignable<NonMovableClass>::value);

    // 测试const类型
    TEST_ASSERT_FALSE(is_nothrow_move_assignable<const int>::value);

    printf("✓ test_is_nothrow_move_assignable passed\n");
#endif
}

#ifdef __cplusplus
}
#endif
}


// ============ 主函数 ============
int test_type_traits(void) {
    // 初始化Unity测试框架
    UNITY_BEGIN();

    printf("\n=== MSD Type Traits Unity Tests ===\n\n");

    // 运行所有类型特性测试
    RUN_TEST(msd_type_traits_unity_test::test_remove_cv);
    RUN_TEST(msd_type_traits_unity_test::test_void_t);
    RUN_TEST(msd_type_traits_unity_test::test_add_rvalue_reference);
    RUN_TEST(msd_type_traits_unity_test::test_enable_if);
    RUN_TEST(msd_type_traits_unity_test::test_conditional);
    RUN_TEST(msd_type_traits_unity_test::test_constant_and_types);
    RUN_TEST(msd_type_traits_unity_test::test_conjunction);
    RUN_TEST(msd_type_traits_unity_test::test_disjunction);
    RUN_TEST(msd_type_traits_unity_test::test_negation);
    RUN_TEST(msd_type_traits_unity_test::test_is_integral);
    RUN_TEST(msd_type_traits_unity_test::test_is_bool);
    RUN_TEST(msd_type_traits_unity_test::test_is_floating_point);
    RUN_TEST(msd_type_traits_unity_test::test_is_same);
    RUN_TEST(msd_type_traits_unity_test::test_is_move_constructible);
    RUN_TEST(msd_type_traits_unity_test::test_is_nothrow_move_constructible);
    RUN_TEST(msd_type_traits_unity_test::test_is_move_assignable);
    RUN_TEST(msd_type_traits_unity_test::test_is_nothrow_move_assignable);

    // 结束测试
    return UNITY_END();
}