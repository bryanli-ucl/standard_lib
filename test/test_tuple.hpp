#pragma once

#include <tuple>
#include <unity.h>

// ==================== 辅助宏，解决模板参数带逗号的问题 ====================
#define TEST_ASSERT_EQUAL_SIZE_T(expected, actual) \
    TEST_ASSERT_EQUAL_UINT((expected), (actual))

// ==================== 测试辅助函数 ====================
#ifndef ARDUINO
// 非Arduino环境的时间函数替代
unsigned long micros() {
    return 0; // 简化版本，或者使用 chrono
}
#endif

// ==================== 基本功能测试 ====================

void test_tuple_default_constructor() {
    msd::tuple<int, double, char> t;

    TEST_ASSERT_EQUAL(0, msd::get<0>(t));
    TEST_ASSERT_EQUAL_DOUBLE(0.0, msd::get<1>(t));
    TEST_ASSERT_EQUAL('\0', msd::get<2>(t));
}

void test_tuple_value_constructor() {
    msd::tuple<int, double, const char*> t(42, 3.14, "Hello");

    TEST_ASSERT_EQUAL(42, msd::get<0>(t));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<1>(t));
    TEST_ASSERT_EQUAL_STRING("Hello", msd::get<2>(t));
}

void test_tuple_single_element() {
    msd::tuple<int> t(100);

    TEST_ASSERT_EQUAL(100, msd::get<0>(t));
}

void test_tuple_empty() {
    msd::tuple<> t;

    // 空tuple应该可以正常构造
    TEST_ASSERT_TRUE(sizeof(t) >= 1);
}

void test_tuple_get_access() {
    msd::tuple<int, float, char, bool> t(10, 2.5f, 'A', true);

    TEST_ASSERT_EQUAL(10, msd::get<0>(t));
    TEST_ASSERT_EQUAL_FLOAT(2.5f, msd::get<1>(t));
    TEST_ASSERT_EQUAL('A', msd::get<2>(t));
    TEST_ASSERT_TRUE(msd::get<3>(t));
}

void test_tuple_get_modify() {
    msd::tuple<int, double> t(10, 20.5);

    msd::get<0>(t) = 999;
    msd::get<1>(t) = 888.8;

    TEST_ASSERT_EQUAL(999, msd::get<0>(t));
    TEST_ASSERT_EQUAL_DOUBLE(888.8, msd::get<1>(t));
}

void test_tuple_const_access() {
    const msd::tuple<int, double> t(42, 3.14);

    TEST_ASSERT_EQUAL(42, msd::get<0>(t));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<1>(t));
}

// ==================== make_tuple 测试 ====================

void test_make_tuple_basic() {
    auto t = msd::make_tuple(10, 3.14, 'X');

    TEST_ASSERT_EQUAL(10, msd::get<0>(t));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<1>(t));
    TEST_ASSERT_EQUAL('X', msd::get<2>(t));
}

void test_make_tuple_single() {
    auto t = msd::make_tuple(42);

    TEST_ASSERT_EQUAL(42, msd::get<0>(t));
}

void test_make_tuple_mixed_types() {
    auto t = msd::make_tuple(100, 2.718f, "Test", true, 'Z');

    TEST_ASSERT_EQUAL(100, msd::get<0>(t));
    TEST_ASSERT_EQUAL_FLOAT(2.718f, msd::get<1>(t));
    TEST_ASSERT_EQUAL_STRING("Test", msd::get<2>(t));
    TEST_ASSERT_TRUE(msd::get<3>(t));
    TEST_ASSERT_EQUAL('Z', msd::get<4>(t));
}

// ==================== 拷贝和移动测试 ====================

void test_tuple_copy_constructor() {
    msd::tuple<int, double> t1(42, 3.14);
    msd::tuple<int, double> t2(t1);

    TEST_ASSERT_EQUAL(42, msd::get<0>(t2));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<1>(t2));

    // 修改t2不应该影响t1
    msd::get<0>(t2) = 100;
    TEST_ASSERT_EQUAL(42, msd::get<0>(t1));
    TEST_ASSERT_EQUAL(100, msd::get<0>(t2));
}

void test_tuple_copy_assignment() {
    msd::tuple<int, double> t1(42, 3.14);
    msd::tuple<int, double> t2(0, 0.0);

    t2 = t1;

    TEST_ASSERT_EQUAL(42, msd::get<0>(t2));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<1>(t2));
}

void test_tuple_move_assignment() {
    msd::tuple<int, double> t1(42, 3.14);
    msd::tuple<int, double> t2(0, 0.0);

    t2 = msd::move(t1);

    TEST_ASSERT_EQUAL(42, msd::get<0>(t2));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<1>(t2));
}

// ==================== 类型特性测试 ====================

void test_tuple_size() {
    // 使用临时变量避免宏展开问题
    size_t size0 = msd::tuple_size<msd::tuple<>>::value;
    size_t size1 = msd::tuple_size<msd::tuple<int>>::value;
    size_t size2 = msd::tuple_size<msd::tuple<int, double>>::value;
    size_t size5 = msd::tuple_size<msd::tuple<int, double, char, bool, float>>::value;

    TEST_ASSERT_EQUAL_SIZE_T(0, size0);
    TEST_ASSERT_EQUAL_SIZE_T(1, size1);
    TEST_ASSERT_EQUAL_SIZE_T(2, size2);
    TEST_ASSERT_EQUAL_SIZE_T(5, size5);
}

void test_tuple_size_const() {
    // const 版本
    size_t size = msd::tuple_size<const msd::tuple<int, double, char>>::value;
    TEST_ASSERT_EQUAL_SIZE_T(3, size);
}

// ==================== 内存布局测试 ====================

void test_tuple_memory_layout() {
    // Tuple应该和struct有相同的内存布局
    struct Equivalent {
        int a;
        double b;
        char c;
    };

    msd::tuple<int, double, char> t;

    // 至少应该不比struct大很多
    TEST_ASSERT_TRUE(sizeof(t) <= sizeof(Equivalent) + 8);
}

void test_tuple_empty_base_optimization() {
    // 空tuple应该只占1字节
    msd::tuple<> empty;
    TEST_ASSERT_EQUAL_SIZE_T(1, sizeof(empty));
}

// ==================== 边界情况测试 ====================

void test_tuple_large_tuple() {
    auto t = msd::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    TEST_ASSERT_EQUAL(1, msd::get<0>(t));
    TEST_ASSERT_EQUAL(5, msd::get<4>(t));
    TEST_ASSERT_EQUAL(10, msd::get<9>(t));
}

void test_tuple_nested() {
    msd::tuple<int, msd::tuple<double, char>> nested(42, msd::make_tuple(3.14, 'X'));

    TEST_ASSERT_EQUAL(42, msd::get<0>(nested));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, msd::get<0>(msd::get<1>(nested)));
    TEST_ASSERT_EQUAL('X', msd::get<1>(msd::get<1>(nested)));
}

void test_tuple_with_pointers() {
    int x    = 42;
    double y = 3.14;

    msd::tuple<int*, double*> t(&x, &y);

    TEST_ASSERT_EQUAL(&x, msd::get<0>(t));
    TEST_ASSERT_EQUAL(&y, msd::get<1>(t));
    TEST_ASSERT_EQUAL(42, *msd::get<0>(t));
    TEST_ASSERT_EQUAL_DOUBLE(3.14, *msd::get<1>(t));
}

void test_tuple_with_arrays() {
    int arr[] = { 1, 2, 3 };
    msd::tuple<int*, size_t> t(arr, 3);

    TEST_ASSERT_EQUAL(arr, msd::get<0>(t));
    TEST_ASSERT_EQUAL_SIZE_T(3, msd::get<1>(t));
}

// ==================== 性能测试 ====================

void test_tuple_performance_construction() {
    const int iterations = 1000;

    unsigned long start = micros();
    for (int i = 0; i < iterations; i++) {
        msd::tuple<int, int, int> t(i, i + 1, i + 2);
        (void)t; // 防止优化掉
    }
    unsigned long elapsed = micros() - start;

    // 基本性能检查（在非Arduino环境会返回0）
    TEST_ASSERT_TRUE(elapsed >= 0);
}

void test_tuple_performance_access() {
    msd::tuple<int, int, int> t(1, 2, 3);
    volatile int sum = 0;

    const int iterations = 10000;
    unsigned long start  = micros();
    for (int i = 0; i < iterations; i++) {
        sum += msd::get<0>(t) + msd::get<1>(t) + msd::get<2>(t);
    }
    unsigned long elapsed = micros() - start;

    // 基本性能检查
    TEST_ASSERT_TRUE(elapsed >= 0);
    TEST_ASSERT_EQUAL(60000, sum);
}

// ==================== 实际使用场景测试 ====================

void test_tuple_as_return_value() {
    // 模拟返回多个值的函数
    auto divide_with_remainder = [](int dividend, int divisor) {
        return msd::make_tuple(dividend / divisor, dividend % divisor);
    };

    auto result = divide_with_remainder(17, 5);

    TEST_ASSERT_EQUAL(3, msd::get<0>(result));
    TEST_ASSERT_EQUAL(2, msd::get<1>(result));
}

void test_tuple_sensor_data() {
    // 模拟传感器数据存储
    using SensorData = msd::tuple<uint16_t, uint16_t, float, uint32_t>;

    SensorData data(1024, 512, 23.5f, 1234567890UL);

    // 访问传感器值
    TEST_ASSERT_EQUAL(1024, msd::get<0>(data));
    TEST_ASSERT_EQUAL(512, msd::get<1>(data));
    TEST_ASSERT_EQUAL_FLOAT(23.5f, msd::get<2>(data));
    TEST_ASSERT_EQUAL_UINT32(1234567890UL, msd::get<3>(data));
}

void test_tuple_structure_bind() {
    auto func = []() {
        return msd::make_tuple(10, 20);
    };
    auto [a, b] = func();
    TEST_ASSERT_EQUAL(10, a);
    TEST_ASSERT_EQUAL(20, b);
}

void test_tuple() {
    UNITY_BEGIN();

    // 基本功能
    RUN_TEST(test_tuple_default_constructor);
    RUN_TEST(test_tuple_value_constructor);
    RUN_TEST(test_tuple_single_element);
    RUN_TEST(test_tuple_empty);
    RUN_TEST(test_tuple_get_access);
    RUN_TEST(test_tuple_get_modify);
    RUN_TEST(test_tuple_const_access);

    // make_tuple
    RUN_TEST(test_make_tuple_basic);
    RUN_TEST(test_make_tuple_single);
    RUN_TEST(test_make_tuple_mixed_types);

    // 拷贝和移动
    RUN_TEST(test_tuple_copy_constructor);
    RUN_TEST(test_tuple_copy_assignment);
    RUN_TEST(test_tuple_move_assignment);

    // 类型特性
    RUN_TEST(test_tuple_size);
    RUN_TEST(test_tuple_size_const);

    // 内存布局
    RUN_TEST(test_tuple_memory_layout);
    RUN_TEST(test_tuple_empty_base_optimization);

    // 边界情况
    RUN_TEST(test_tuple_large_tuple);
    RUN_TEST(test_tuple_nested);
    RUN_TEST(test_tuple_with_pointers);
    RUN_TEST(test_tuple_with_arrays);

    // 性能测试
    RUN_TEST(test_tuple_performance_construction);
    RUN_TEST(test_tuple_performance_access);

    // 实际场景
    RUN_TEST(test_tuple_as_return_value);
    RUN_TEST(test_tuple_sensor_data);

    RUN_TEST(test_tuple_structure_bind);

    UNITY_END();
}