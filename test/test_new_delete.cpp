#include <unity.h>

#include <avr-memory.hpp>

void test_basic_new_delete(void) {
    // 测试基本类型
    int* int_ptr = new int(42);
    TEST_ASSERT_NOT_NULL(int_ptr);
    TEST_ASSERT_EQUAL(42, *int_ptr);
    delete int_ptr;

    // 测试对象
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
    };

    Point* point = new Point(10, 20);
    TEST_ASSERT_NOT_NULL(point);
    TEST_ASSERT_EQUAL(10, point->x);
    TEST_ASSERT_EQUAL(20, point->y);
    delete point;

    // 测试默认构造
    int* default_int = new int;
    TEST_ASSERT_NOT_NULL(default_int);
    *default_int = 100;
    TEST_ASSERT_EQUAL(100, *default_int);
    delete default_int;
}

// 测试 2: 数组 new/delete
void test_array_new_delete(void) {
    // 测试基本类型数组
    const size_t array_size = 5;
    int* int_array          = new int[array_size];
    TEST_ASSERT_NOT_NULL(int_array);

    // 初始化数组
    for (size_t i = 0; i < array_size; i++) {
        int_array[i] = static_cast<int>(i * 10);
    }

    // 验证数组内容
    for (size_t i = 0; i < array_size; i++) {
        TEST_ASSERT_EQUAL(static_cast<int>(i * 10), int_array[i]);
    }

    delete[] int_array;

    // 测试对象数组
    struct Simple {
        int value;
        Simple() : value(0) {}
        Simple(int v) : value(v) {}
    };

    Simple* obj_array = new Simple[3];
    TEST_ASSERT_NOT_NULL(obj_array);

    // 初始化对象数组
    for (int i = 0; i < 3; i++) {
        obj_array[i].value = i * 100;
    }

    // 验证
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL(i * 100, obj_array[i].value);
    }

    delete[] obj_array;
}

// 测试 3: placement new
void test_placement_new(void) {
    // 为 placement new 预分配内存
    alignas(int) unsigned char buffer[sizeof(int) * 2];

    // 使用 placement new 在 buffer 中构造对象
    int* ptr1 = new (buffer) int(123);
    TEST_ASSERT_EQUAL_PTR(buffer, ptr1);
    TEST_ASSERT_EQUAL(123, *ptr1);

    // 在 buffer 的偏移位置构造第二个对象
    int* ptr2 = new (buffer + sizeof(int)) int(456);
    TEST_ASSERT_EQUAL_PTR(buffer + sizeof(int), ptr2);
    TEST_ASSERT_EQUAL(456, *ptr2);

    // 验证两个对象不重叠
    TEST_ASSERT_NOT_EQUAL(ptr1, ptr2);
    TEST_ASSERT_EQUAL(123, *ptr1);
    TEST_ASSERT_EQUAL(456, *ptr2);

    // 注意：placement new 构造的对象需要显式调用析构函数（如果有的话）
    // 对于 POD 类型（如 int），不需要调用析构函数
}

// 测试 4: 内存分配失败处理
void test_memory_allocation_failure(void) {
    // 这个测试取决于你的内存管理器
    // 你可以测试分配过大内存的情况

    // 尝试分配一个非常大的内存块
    size_t huge_size = 1024 * 1024 * 1024; // 1GB - 在单片机中肯定失败
    void* huge_ptr   = operator new(huge_size);

    // 应该返回 nullptr
    TEST_ASSERT_NULL(huge_ptr);

    // 如果使用普通的 new，在某些配置下可能会进入错误处理
    // 这取决于你的 operator new 实现
}

// 测试 5: nothrow new
void test_nothrow_new(void) {
    // 测试不抛出异常的 new
    int* ptr = new int(999);
    TEST_ASSERT_NOT_NULL(ptr);
    if (ptr) {
        TEST_ASSERT_EQUAL(999, *ptr);
        delete ptr;
    }

    // 测试 nothrow 数组
    int* arr = new int[10];
    TEST_ASSERT_NOT_NULL(arr);
    if (arr) {
        for (int i = 0; i < 10; i++) {
            arr[i] = i;
        }
        delete[] arr;
    }
}

// 测试 6: 多次分配和释放
void test_multiple_allocations(void) {
    const int num_allocations = 10;
    int* pointers[num_allocations];

    // 分配多个内存块
    for (int i = 0; i < num_allocations; i++) {
        pointers[i] = new int(i * 100);
        TEST_ASSERT_NOT_NULL(pointers[i]);
        TEST_ASSERT_EQUAL(i * 100, *pointers[i]);
    }

    // 验证所有指针都不同
    for (int i = 0; i < num_allocations; i++) {
        for (int j = i + 1; j < num_allocations; j++) {
            TEST_ASSERT_NOT_EQUAL(pointers[i], pointers[j]);
        }
    }

    // 释放所有内存块
    for (int i = 0; i < num_allocations; i++) {
        delete pointers[i];
    }
}

// 测试 7: 零字节分配
void test_zero_size_allocation(void) {
    // 零字节分配应该返回非空指针（或空指针，取决于实现）
    void* ptr = operator new(0);

    // C++ 标准允许零字节分配返回一个唯一的指针
    // 但实际使用中应该避免这种情况
    TEST_ASSERT_NOT_NULL(ptr);

    operator delete(ptr);

    // 测试数组零字节分配
    void* arr_ptr = operator new[](0);
    TEST_ASSERT_NOT_NULL(arr_ptr);
    operator delete[](arr_ptr);
}