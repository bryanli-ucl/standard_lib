#pragma once

#include <pair>
#include <unity.h>
#include <vector>

// 测试基本构造函数
void test_pair_default_constructor() {
    msd::pair<int, int> p;
    TEST_ASSERT_EQUAL(0, p.first);
    TEST_ASSERT_EQUAL(0, p.second);
}

void test_pair_value_constructor() {
    msd::pair<int, const char*> p(42, "hello");
    TEST_ASSERT_EQUAL(42, p.first);
    TEST_ASSERT_EQUAL_STRING("hello", p.second);
}

void test_pair_copy_constructor() {
    msd::pair<int, double> p1(10, 3.14);
    msd::pair<int, double> p2(p1);
    TEST_ASSERT_EQUAL(10, p2.first);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, p2.second);
}

void test_pair_move_constructor() {
    msd::pair<int, float> p1(100, 2.5f);
    msd::pair<int, float> p2(msd::move(p1));
    TEST_ASSERT_EQUAL(100, p2.first);
    TEST_ASSERT_EQUAL_FLOAT(2.5f, p2.second);
}

// 测试赋值操作
void test_pair_copy_assignment() {
    msd::pair<int, char> p1(5, 'a');
    msd::pair<int, char> p2;
    p2 = p1;
    TEST_ASSERT_EQUAL(5, p2.first);
    TEST_ASSERT_EQUAL('a', p2.second);
}

void test_pair_move_assignment() {
    msd::pair<long, double> p1(200, 99.99);
    msd::pair<long, double> p2;
    p2 = msd::move(p1);
    TEST_ASSERT_EQUAL(200, p2.first);
    TEST_ASSERT_EQUAL_DOUBLE(99.99, p2.second);
}

void test_pair_self_assignment() {
    msd::pair<int, int> p(10, 20);
    p = p;
    TEST_ASSERT_EQUAL(10, p.first);
    TEST_ASSERT_EQUAL(20, p.second);
}

void test_pair_self_move_assignment() {
    msd::pair<int, int> p(10, 20);
    p = msd::move(p);
    TEST_ASSERT_EQUAL(10, p.first);
    TEST_ASSERT_EQUAL(20, p.second);
}

// 测试 make_pair
void test_make_pair() {
    auto p = msd::make_pair(42, 3.14);
    TEST_ASSERT_EQUAL(42, p.first);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, p.second);
}

void test_make_pair_type_deduction() {
    int x  = 100;
    auto p = msd::make_pair(x, 'c');
    TEST_ASSERT_EQUAL(100, p.first);
    TEST_ASSERT_EQUAL('c', p.second);
}

void test_make_pair_with_pointers() {
    int x  = 10;
    auto p = msd::make_pair(&x, "test");
    TEST_ASSERT_EQUAL(&x, p.first);
    TEST_ASSERT_EQUAL_STRING("test", p.second);
}

void test_make_pair_rvalue() {
    auto p = msd::make_pair(42, 3.14);
    TEST_ASSERT_EQUAL(42, p.first);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, p.second);
}

// 测试 swap
void test_pair_swap() {
    msd::pair<int, char> p1(1, 'a');
    msd::pair<int, char> p2(2, 'b');

    p1.swap(p2);

    TEST_ASSERT_EQUAL(2, p1.first);
    TEST_ASSERT_EQUAL('b', p1.second);
    TEST_ASSERT_EQUAL(1, p2.first);
    TEST_ASSERT_EQUAL('a', p2.second);
}

void test_pair_msd_swap() {
    msd::pair<int, double> p1(10, 1.5);
    msd::pair<int, double> p2(20, 2.5);

    msd::swap(p1, p2);

    TEST_ASSERT_EQUAL(20, p1.first);
    TEST_ASSERT_EQUAL_DOUBLE(2.5, p1.second);
    TEST_ASSERT_EQUAL(10, p2.first);
    TEST_ASSERT_EQUAL_DOUBLE(1.5, p2.second);
}

// 测试嵌套 pair
void test_pair_nested() {
    msd::pair<int, msd::pair<int, int>> p(1, msd::make_pair(2, 3));

    TEST_ASSERT_EQUAL(1, p.first);
    TEST_ASSERT_EQUAL(2, p.second.first);
    TEST_ASSERT_EQUAL(3, p.second.second);
}

void test_pair_deeply_nested() {
    msd::pair<msd::pair<int, int>, msd::pair<int, int>> p(
    msd::make_pair(1, 2),
    msd::make_pair(3, 4));

    TEST_ASSERT_EQUAL(1, p.first.first);
    TEST_ASSERT_EQUAL(2, p.first.second);
    TEST_ASSERT_EQUAL(3, p.second.first);
    TEST_ASSERT_EQUAL(4, p.second.second);
}

// 测试不同类型的 pair
void test_pair_different_types() {
    msd::pair<char, bool> p1('A', true);
    msd::pair<double, float> p2(3.14159, 2.71f);
    msd::pair<int*, const char*> p3(nullptr, "test");

    TEST_ASSERT_EQUAL('A', p1.first);
    TEST_ASSERT_TRUE(p1.second);
    TEST_ASSERT_EQUAL_DOUBLE(3.14159, p2.first);
    TEST_ASSERT_EQUAL_FLOAT(2.71f, p2.second);
    TEST_ASSERT_NULL(p3.first);
    TEST_ASSERT_EQUAL_STRING("test", p3.second);
}

void test_pair_with_arrays() {
    int arr[] = { 1, 2, 3 };
    msd::pair<int*, int> p(arr, 3);

    TEST_ASSERT_EQUAL(arr, p.first);
    TEST_ASSERT_EQUAL(3, p.second);
    TEST_ASSERT_EQUAL(1, p.first[0]);
    TEST_ASSERT_EQUAL(2, p.first[1]);
    TEST_ASSERT_EQUAL(3, p.first[2]);
}

// 测试 pair 作为返回值
msd::pair<int, int> divide_with_remainder(int a, int b) {
    return msd::make_pair(a / b, a % b);
}

void test_pair_as_return_value() {
    auto result = divide_with_remainder(17, 5);
    TEST_ASSERT_EQUAL(3, result.first);  // 商
    TEST_ASSERT_EQUAL(2, result.second); // 余数
}

msd::pair<int, int> min_max(int a, int b) {
    if (a < b) {
        return msd::make_pair(a, b);
    } else {
        return msd::make_pair(b, a);
    }
}

void test_pair_return_min_max() {
    auto result1 = min_max(10, 20);
    TEST_ASSERT_EQUAL(10, result1.first);
    TEST_ASSERT_EQUAL(20, result1.second);

    auto result2 = min_max(30, 15);
    TEST_ASSERT_EQUAL(15, result2.first);
    TEST_ASSERT_EQUAL(30, result2.second);
}

// 测试指针类型的 pair
void test_pair_pointers() {
    int x = 10, y = 20;
    msd::pair<int*, int*> p(&x, &y);

    TEST_ASSERT_EQUAL(&x, p.first);
    TEST_ASSERT_EQUAL(&y, p.second);
    TEST_ASSERT_EQUAL(10, *p.first);
    TEST_ASSERT_EQUAL(20, *p.second);

    *p.first  = 100;
    *p.second = 200;

    TEST_ASSERT_EQUAL(100, x);
    TEST_ASSERT_EQUAL(200, y);
}

// 测试 pair 与 C 字符串
void test_pair_cstring() {
    msd::pair<const char*, int> p("hello", 5);

    TEST_ASSERT_EQUAL_STRING("hello", p.first);
    TEST_ASSERT_EQUAL(5, p.second);
}

void test_pair_multiple_cstrings() {
    msd::pair<const char*, const char*> p("hello", "world");

    TEST_ASSERT_EQUAL_STRING("hello", p.first);
    TEST_ASSERT_EQUAL_STRING("world", p.second);
}

// 测试数值类型
void test_pair_numeric_types() {
    msd::pair<short, long> p1(100, 1000000L);
    msd::pair<unsigned int, unsigned long> p2(500u, 999999ul);
    msd::pair<float, double> p3(1.5f, 2.5);

    TEST_ASSERT_EQUAL(100, p1.first);
    TEST_ASSERT_EQUAL(1000000L, p1.second);
    TEST_ASSERT_EQUAL_UINT(500, p2.first);
    TEST_ASSERT_EQUAL_UINT(999999, p2.second);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, p3.first);
    TEST_ASSERT_EQUAL_DOUBLE(2.5, p3.second);
}

void test_pair_signed_unsigned() {
    msd::pair<int, unsigned int> p(-10, 20u);
    TEST_ASSERT_EQUAL(-10, p.first);
    TEST_ASSERT_EQUAL_UINT(20, p.second);
}

// 测试 type alias
void test_pair_type_aliases() {
    using IntDoublePair = msd::pair<int, double>;

    IntDoublePair p(42, 3.14);

    // 验证类型别名
    IntDoublePair::type_1 x = p.first;
    IntDoublePair::type_2 y = p.second;

    TEST_ASSERT_EQUAL(42, x);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, y);
}

// 测试 forward constructor
void test_pair_forward_constructor() {
    int a    = 10;
    double b = 20.5;

    // 使用左值
    msd::pair<int, double> p1(a, b);
    TEST_ASSERT_EQUAL(10, p1.first);
    TEST_ASSERT_EQUAL_DOUBLE(20.5, p1.second);

    // 使用右值
    msd::pair<int, double> p2(100, 200.5);
    TEST_ASSERT_EQUAL(100, p2.first);
    TEST_ASSERT_EQUAL_DOUBLE(200.5, p2.second);
}

void test_pair_mixed_value_categories() {
    int x = 5;
    // 混合左值和右值
    msd::pair<int, int> p(x, 10);
    TEST_ASSERT_EQUAL(5, p.first);
    TEST_ASSERT_EQUAL(10, p.second);
}

// 测试 const pair
void test_pair_const() {
    const msd::pair<int, int> p(5, 10);
    TEST_ASSERT_EQUAL(5, p.first);
    TEST_ASSERT_EQUAL(10, p.second);
}

void test_pair_const_members() {
    const int x    = 100;
    const double y = 3.14;
    msd::pair<int, double> p(x, y);

    TEST_ASSERT_EQUAL(100, p.first);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, p.second);
}

// 测试零初始化
void test_pair_zero_initialization() {
    msd::pair<int, int> p;
    TEST_ASSERT_EQUAL(0, p.first);
    TEST_ASSERT_EQUAL(0, p.second);
}

void test_pair_pointer_zero_init() {
    msd::pair<int*, double*> p;
    TEST_ASSERT_NULL(p.first);
    TEST_ASSERT_NULL(p.second);
}

// 测试布尔值
void test_pair_bool_values() {
    msd::pair<bool, bool> p1(true, false);
    msd::pair<bool, bool> p2(false, true);

    TEST_ASSERT_TRUE(p1.first);
    TEST_ASSERT_FALSE(p1.second);
    TEST_ASSERT_FALSE(p2.first);
    TEST_ASSERT_TRUE(p2.second);
}

// 测试大小
void test_pair_sizeof() {
    // 验证 pair 的大小是否合理（至少是两个成员的大小之和）
    TEST_ASSERT_GREATER_OR_EQUAL(sizeof(int) + sizeof(double),
    sizeof(msd::pair<int, double>));
}

// 测试多次赋值
void test_pair_multiple_assignments() {
    msd::pair<int, int> p(1, 2);

    p = msd::make_pair(3, 4);
    TEST_ASSERT_EQUAL(3, p.first);
    TEST_ASSERT_EQUAL(4, p.second);

    p = msd::make_pair(5, 6);
    TEST_ASSERT_EQUAL(5, p.first);
    TEST_ASSERT_EQUAL(6, p.second);
}

// 测试复杂场景
void test_pair_complex_scenario() {
    // 创建 pair 的 vector
    msd::pair<int, const char*> arr[3];
    arr[0] = msd::make_pair(1, "one");
    arr[1] = msd::make_pair(2, "two");
    arr[2] = msd::make_pair(3, "three");

    TEST_ASSERT_EQUAL(1, arr[0].first);
    TEST_ASSERT_EQUAL_STRING("one", arr[0].second);
    TEST_ASSERT_EQUAL(2, arr[1].first);
    TEST_ASSERT_EQUAL_STRING("two", arr[1].second);
    TEST_ASSERT_EQUAL(3, arr[2].first);
    TEST_ASSERT_EQUAL_STRING("three", arr[2].second);
}

// 测试函数指针
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

void test_pair_function_pointers() {
    using FuncPtr = int (*)(int, int);
    msd::pair<FuncPtr, FuncPtr> p(add, multiply);

    TEST_ASSERT_EQUAL(7, p.first(3, 4));
    TEST_ASSERT_EQUAL(12, p.second(3, 4));
}

void test_pair_structed_bind() {
    auto p      = msd::make_pair(10, 20);
    auto [a, b] = p;
    TEST_ASSERT_EQUAL(10, a);
    TEST_ASSERT_EQUAL(20, b);
}

// 主测试函数
void test_pair_basic() {
    RUN_TEST(test_pair_default_constructor);
    RUN_TEST(test_pair_value_constructor);
    RUN_TEST(test_pair_copy_constructor);
    RUN_TEST(test_pair_move_constructor);

    RUN_TEST(test_pair_copy_assignment);
    RUN_TEST(test_pair_move_assignment);
    RUN_TEST(test_pair_self_assignment);
    RUN_TEST(test_pair_self_move_assignment);

    RUN_TEST(test_make_pair);
    RUN_TEST(test_make_pair_type_deduction);
    RUN_TEST(test_make_pair_with_pointers);
    RUN_TEST(test_make_pair_rvalue);

    RUN_TEST(test_pair_swap);
    RUN_TEST(test_pair_msd_swap);

    RUN_TEST(test_pair_nested);
    RUN_TEST(test_pair_deeply_nested);

    RUN_TEST(test_pair_different_types);
    RUN_TEST(test_pair_with_arrays);

    RUN_TEST(test_pair_as_return_value);
    RUN_TEST(test_pair_return_min_max);

    RUN_TEST(test_pair_pointers);
    RUN_TEST(test_pair_cstring);
    RUN_TEST(test_pair_multiple_cstrings);

    RUN_TEST(test_pair_numeric_types);
    RUN_TEST(test_pair_signed_unsigned);

    RUN_TEST(test_pair_type_aliases);
    RUN_TEST(test_pair_forward_constructor);
    RUN_TEST(test_pair_mixed_value_categories);

    RUN_TEST(test_pair_const);
    RUN_TEST(test_pair_const_members);

    RUN_TEST(test_pair_zero_initialization);
    RUN_TEST(test_pair_pointer_zero_init);
    RUN_TEST(test_pair_bool_values);

    RUN_TEST(test_pair_sizeof);
    RUN_TEST(test_pair_multiple_assignments);
    RUN_TEST(test_pair_complex_scenario);
    RUN_TEST(test_pair_function_pointers);

    RUN_TEST(test_pair_structed_bind);
}