#include <unity.h>

#include <array.hpp>

using msd::array;

void test_array_basic() {
    array<int, 50> a;
    TEST_ASSERT_EQUAL(50, a.size());
}

void test_array_access() {
    array<int, 3> arr;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    TEST_ASSERT_EQUAL(1, arr[0]);
    TEST_ASSERT_EQUAL(2, arr[1]);
    TEST_ASSERT_EQUAL(3, arr[2]);
}

void test_array_fill() {
    array<int, 5> arr;
    arr.fill(42);

    for (size_t i = 0; i < arr.size(); i++) {
        TEST_ASSERT_EQUAL(42, arr[i]);
    }
}

void test_array_args_init() {
    array<int, 5> arr(1, 2, 3, 4, 5);
    for (size_t i = 0; i < arr.size(); i++) {
        TEST_ASSERT_EQUAL(i + 1, arr[i]);
    }
}

void test_array_iterator() {
    auto arr = msd::make_array(1, 2, 3, 4, 5);
    size_t i = 1;

    for (const auto& x : arr) {
        TEST_ASSERT_EQUAL(x, i);
        i++;
    }
}

void test_array_make_array() {
    auto arr = msd::make_array(1, 2, 3);
    TEST_ASSERT_EQUAL(3, arr.size());
    TEST_ASSERT_EQUAL(1, arr.at(0));
    TEST_ASSERT_EQUAL(2, arr.at(1));
    TEST_ASSERT_EQUAL(3, arr.at(2));
}
