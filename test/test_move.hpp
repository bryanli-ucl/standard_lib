#pragma once


#include <stdio.h>
#include <string.h>
#include <unity.h>

#include <move>
#include <type_traits>

namespace msd_test {

// 测试辅助类
class MovableOnly {
    public:
    int* data;
    static int move_count;
    static int destruct_count;

    explicit MovableOnly(int value = 0) : data(new int(value)) {}

    // 禁止复制
    MovableOnly(const MovableOnly&)            = delete;
    MovableOnly& operator=(const MovableOnly&) = delete;

    // 允许移动
    MovableOnly(MovableOnly&& other) noexcept : data(other.data) {
        other.data = nullptr;
        move_count++;
    }

    MovableOnly& operator=(MovableOnly&& other) noexcept {
        if (this != &other) {
            delete data;
            data       = other.data;
            other.data = nullptr;
            move_count++;
        }
        return *this;
    }

    ~MovableOnly() {
        delete data;
        destruct_count++;
    }

    int get() const { return data ? *data : -1; }
};

int MovableOnly::move_count     = 0;
int MovableOnly::destruct_count = 0;

// 简单测试类
class TestClass {
    public:
    int value;
    static int copy_count;
    static int move_count;

    explicit TestClass(int v = 0) : value(v) {}

    TestClass(const TestClass& other) : value(other.value) {
        copy_count++;
    }

    TestClass(TestClass&& other) noexcept : value(other.value) {
        other.value = 0;
        move_count++;
    }

    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            value = other.value;
            copy_count++;
        }
        return *this;
    }

    TestClass& operator=(TestClass&& other) noexcept {
        if (this != &other) {
            value       = other.value;
            other.value = 0;
            move_count++;
        }
        return *this;
    }
};

int TestClass::copy_count = 0;
int TestClass::move_count = 0;

} // namespace msd_test

#ifdef __cplusplus
extern "C" {
#endif

// 测试1: 基本类型的move
void test_move_basic_types(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试整数
    int x    = 42;
    int&& r1 = move(x);
    TEST_ASSERT_EQUAL_INT(42, r1);

    // 测试指针
    int* p    = &x;
    int*&& r2 = move(p);
    TEST_ASSERT_EQUAL_PTR(&x, r2);

    // 测试浮点数
    double d    = 3.14159;
    double&& r3 = move(d);
    TEST_ASSERT_EQUAL_DOUBLE(3.14159, r3);

    // 测试常量
    const int y    = 100;
    const int&& r4 = move(y);
    TEST_ASSERT_EQUAL_INT(100, r4);

    printf("✓ test_move_basic_types passed\n");
#endif
}

// 测试2: 自定义类的move
void test_move_custom_class(void) {
#ifdef __cplusplus
    using namespace msd;
    using msd_test::TestClass;

    // 重置计数器
    TestClass::move_count = 0;

    TestClass obj1(10);
    TestClass obj2(move(obj1)); // 应该调用移动构造函数

    TEST_ASSERT_EQUAL_INT(1, TestClass::move_count);
    TEST_ASSERT_EQUAL_INT(10, obj2.value);

    // 测试移动赋值
    TestClass obj3(20);
    TestClass obj4(30);
    obj4 = move(obj3);

    TEST_ASSERT_EQUAL_INT(2, TestClass::move_count); // 增加了1次移动赋值

    printf("✓ test_move_custom_class passed\n");
#endif
}

// 测试3: forward函数
void test_forward_function(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试左值转发
    int x     = 42;
    int& lref = x;
    auto&& f1 = forward<int&>(lref);
    TEST_ASSERT_EQUAL_INT(42, f1);

    // 测试指针转发
    int* p    = &x;
    auto&& f2 = forward<int*>(p);
    TEST_ASSERT_EQUAL_PTR(&x, f2);

    // 测试const转发
    const int y     = 100;
    const int& cref = y;
    auto&& f3       = forward<const int&>(cref);
    TEST_ASSERT_EQUAL_INT(100, f3);

    printf("✓ test_forward_function passed\n");
#endif
}

// 测试4: 基本类型swap
void test_swap_basic_types(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试整数交换
    int a = 10, b = 20;
    swap(a, b);
    TEST_ASSERT_EQUAL_INT(20, a);
    TEST_ASSERT_EQUAL_INT(10, b);

    // 测试浮点数交换
    double x = 3.14, y = 2.71;
    swap(x, y);
    TEST_ASSERT_EQUAL_DOUBLE(2.71, x);
    TEST_ASSERT_EQUAL_DOUBLE(3.14, y);

    // 测试布尔值交换
    bool b1 = true, b2 = false;
    swap(b1, b2);
    TEST_ASSERT_EQUAL(false, b1);
    TEST_ASSERT_EQUAL(true, b2);

    // 测试字符交换
    char c1 = 'A', c2 = 'B';
    swap(c1, c2);
    TEST_ASSERT_EQUAL('B', c1);
    TEST_ASSERT_EQUAL('A', c2);

    printf("✓ test_swap_basic_types passed\n");
#endif
}

// 测试5: 自定义类swap
void test_swap_custom_class(void) {
#ifdef __cplusplus
    using namespace msd;
    using msd_test::TestClass;

    // 重置计数器
    TestClass::copy_count = 0;
    TestClass::move_count = 0;

    TestClass obj1(100);
    TestClass obj2(200);

    swap(obj1, obj2);

    // 验证值已交换
    TEST_ASSERT_EQUAL_INT(200, obj1.value);
    TEST_ASSERT_EQUAL_INT(100, obj2.value);

    // 验证使用了移动语义而非复制
    TEST_ASSERT_EQUAL_INT(0, TestClass::copy_count);
    TEST_ASSERT_EQUAL_INT(3, TestClass::move_count); // 临时对象+两次移动

    // 测试可移动不可复制类型
    using msd_test::MovableOnly;
    MovableOnly::move_count = 0;

    MovableOnly m1(42);
    MovableOnly m2(84);

    swap(m1, m2);

    TEST_ASSERT_EQUAL_INT(84, m1.get());
    TEST_ASSERT_EQUAL_INT(42, m2.get());

    printf("✓ test_swap_custom_class passed\n");
#endif
}

// 测试6: 数组swap
void test_swap_array(void) {
#ifdef __cplusplus
    using namespace msd;

    // 测试整数数组元素交换
    int arr1[5] = { 1, 2, 3, 4, 5 };
    int arr2[5] = { 6, 7, 8, 9, 10 };

    // 逐个元素交换
    for (int i = 0; i < 5; i++) {
        swap(arr1[i], arr2[i]);
    }

    // 验证交换结果
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(i + 6, arr1[i]);
        TEST_ASSERT_EQUAL_INT(i + 1, arr2[i]);
    }

    // 测试字符数组
    char str1[10] = "Hello";
    char str2[10] = "World";

    // 交换字符串内容
    swap(str1[0], str2[0]);
    TEST_ASSERT_EQUAL('W', str1[0]);
    TEST_ASSERT_EQUAL('H', str2[0]);

    printf("✓ test_swap_array passed\n");
#endif
}

// 测试7: remove_reference类型特性
void test_remove_reference(void) {
#ifdef __cplusplus
    using namespace msd;

    // 静态断言在编译时检查
    // 这些检查需要编译时验证，我们在运行时通过typeid验证

    // 验证类型正确性
    // 注意：这不是标准的Unity测试方式，但对于类型特性是必要的

    TEST_ASSERT_TRUE((msd::is_same<msd::remove_reference<int>::type, int>::value));
    TEST_ASSERT_TRUE((msd::is_same<msd::remove_reference<int&>::type, int>::value));
    TEST_ASSERT_TRUE((msd::is_same<msd::remove_reference<int&&>::type, int>::value));
    TEST_ASSERT_TRUE((msd::is_same<msd::remove_reference<const int>::type, const int>::value));
    TEST_ASSERT_TRUE((msd::is_same<msd::remove_reference<const int&>::type, const int>::value));
#endif
}

#ifdef __cplusplus
}
#endif
