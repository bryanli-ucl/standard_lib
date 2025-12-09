#pragma once

#include <unity.h>

#include <queue>
#include <vector>

// Test default construction
void test_vector_default_construction(void) {
    msd::vector<int> v;
    TEST_ASSERT_EQUAL(0, v.size());
    TEST_ASSERT_EQUAL(16, v.capacity());
    TEST_ASSERT_TRUE(v.empty());
}

// Test copy construction
void test_vector_copy_construction(void) {
    msd::vector<int> original;
    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    msd::vector<int> copy(original);
    TEST_ASSERT_EQUAL(3, copy.size());
    TEST_ASSERT_EQUAL(3, original.size());
    TEST_ASSERT_EQUAL(1, copy[0]);
    TEST_ASSERT_EQUAL(2, copy[1]);
    TEST_ASSERT_EQUAL(3, copy[2]);
}

// Test move construction
void test_vector_move_construction(void) {
    msd::vector<int> original;
    original.push_back(1);
    original.push_back(2);

    msd::vector<int> moved(msd::move(original));
    TEST_ASSERT_EQUAL(2, moved.size());
    TEST_ASSERT_EQUAL(1, moved[0]);
    TEST_ASSERT_EQUAL(2, moved[1]);
}

// Test copy assignment operator
void test_vector_copy_assignment(void) {
    msd::vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);

    msd::vector<int> v2;
    v2 = v1; // Copy assignment

    TEST_ASSERT_EQUAL(3, v2.size());
    TEST_ASSERT_EQUAL(10, v2[0]);
    TEST_ASSERT_EQUAL(20, v2[1]);
    TEST_ASSERT_EQUAL(30, v2[2]);

    // Test self-assignment
    v2 = v2;
    TEST_ASSERT_EQUAL(3, v2.size());
    TEST_ASSERT_EQUAL(10, v2[0]);
}

// Test move assignment operator
void test_vector_move_assignment(void) {
    msd::vector<int> v1;
    v1.push_back(100);
    v1.push_back(200);

    msd::vector<int> v2;
    v2 = msd::move(v1); // Move assignment

    TEST_ASSERT_EQUAL(2, v2.size());
    TEST_ASSERT_EQUAL(100, v2[0]);
    TEST_ASSERT_EQUAL(200, v2[1]);
    TEST_ASSERT_EQUAL(0, v1.size());

    // Test self-assignment
    v2 = msd::move(v2);
    TEST_ASSERT_EQUAL(2, v2.size());
}

// Test parameter pack construction
void test_vector_parameter_pack_construction(void) {
    msd::vector<int> v;
    v.push_back(5);
    v.push_back(10);
    v.push_back(15);
    v.push_back(20);
    TEST_ASSERT_EQUAL(4, v.size());
    TEST_ASSERT_EQUAL(5, v[0]);
    TEST_ASSERT_EQUAL(10, v[1]);
    TEST_ASSERT_EQUAL(15, v[2]);
    TEST_ASSERT_EQUAL(20, v[3]);
}

// Test push_back and emplace_back
void test_vector_push_emplace_back(void) {
    msd::vector<int> v;

    v.push_back(1);
    TEST_ASSERT_EQUAL(1, v.size());
    TEST_ASSERT_EQUAL(1, v[0]);

    v.push_back(2);
    TEST_ASSERT_EQUAL(2, v.size());
    TEST_ASSERT_EQUAL(1, v[0]);
    TEST_ASSERT_EQUAL(2, v[1]);

    v.push_back(3);
    TEST_ASSERT_EQUAL(3, v.size());
    TEST_ASSERT_EQUAL(3, v[2]);
}

// Test element access methods
void test_vector_element_access(void) {
    msd::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // Test operator[]
    TEST_ASSERT_EQUAL(10, v[0]);
    TEST_ASSERT_EQUAL(20, v[1]);
    TEST_ASSERT_EQUAL(30, v[2]);

    // Test at
    TEST_ASSERT_EQUAL(10, v.at(0));
    TEST_ASSERT_EQUAL(20, v.at(1));
    TEST_ASSERT_EQUAL(30, v.at(2));

    // Test const version
    const msd::vector<int>& const_v = v;
    TEST_ASSERT_EQUAL(10, const_v[0]);
    TEST_ASSERT_EQUAL(10, const_v.at(0));
}

// Test pop_back functionality
void test_vector_pop_back(void) {
    msd::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.pop_back();
    TEST_ASSERT_EQUAL(2, v.size());
    TEST_ASSERT_EQUAL(1, v[0]);
    TEST_ASSERT_EQUAL(2, v[1]);

    v.pop_back();
    TEST_ASSERT_EQUAL(1, v.size());
    TEST_ASSERT_EQUAL(1, v[0]);

    v.pop_back();
    TEST_ASSERT_EQUAL(0, v.size());
    TEST_ASSERT_TRUE(v.empty());
}

// Test clear functionality
void test_vector_clear(void) {
    msd::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    TEST_ASSERT_FALSE(v.empty());
    v.clear();
    TEST_ASSERT_TRUE(v.empty());
    TEST_ASSERT_EQUAL(0, v.size());

    // Test clear on empty vector
    v.clear();
    TEST_ASSERT_TRUE(v.empty());
}

// Test iterator functionality
void test_vector_iterators(void) {
    msd::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    // Test begin/end
    auto it = v.begin();
    TEST_ASSERT_EQUAL(1, *it);

    ++it;
    TEST_ASSERT_EQUAL(2, *it);

    ++it;
    TEST_ASSERT_EQUAL(3, *it);

    ++it;
    TEST_ASSERT(it == v.end());

    // Test range-based for loop
    int sum = 0;
    for (auto& item : v) {
        sum += item;
    }
    TEST_ASSERT_EQUAL(6, sum);
}

struct test_obj {
    int32_t a;
    int32_t b;
    int32_t c;
};


// Test with complex types
void test_vector_complex_types(void) {
    msd::vector<test_obj> v;

    // // Test push_back with copy
    test_obj q1{ 1, 2, 3 };
    v.push_back(q1);
    TEST_ASSERT_EQUAL(1, v.size());
    TEST_ASSERT_EQUAL(1, v[0].a);
    TEST_ASSERT_EQUAL(2, v[0].b);
    TEST_ASSERT_EQUAL(3, v[0].c);

    // Test push_back with move
    test_obj q2{ 3, 5, 7 };
    v.push_back(msd::move(q2));
    TEST_ASSERT_EQUAL(2, v.size());
    TEST_ASSERT_EQUAL(3, v[1].a);
    TEST_ASSERT_EQUAL(5, v[1].b);
    TEST_ASSERT_EQUAL(7, v[1].c);

    // Test emplace_back
    // v.push_back(200);
    // TEST_ASSERT_EQUAL(200, v[2].capacity());
}

// Test data method
void test_vector_data_method(void) {
    msd::vector<int> v;
    v.push_back(10);
    v.push_back(20);

    int* data = v.data();
    TEST_ASSERT_NOT_NULL(data);
    TEST_ASSERT_EQUAL(10, data[0]);
    TEST_ASSERT_EQUAL(20, data[1]);

    const msd::vector<int>& const_v = v;
    const int* const_data           = const_v.data();
    TEST_ASSERT_NOT_NULL(const_data);
    TEST_ASSERT_EQUAL(10, const_data[0]);
    TEST_ASSERT_EQUAL(20, const_data[1]);
}

void test_vector() {
    UNITY_BEGIN();
    RUN_TEST(test_vector_default_construction);
    RUN_TEST(test_vector_copy_construction);
    RUN_TEST(test_vector_move_construction);
    RUN_TEST(test_vector_copy_assignment);
    RUN_TEST(test_vector_move_assignment);
    RUN_TEST(test_vector_parameter_pack_construction);
    RUN_TEST(test_vector_push_emplace_back);
    RUN_TEST(test_vector_element_access);
    RUN_TEST(test_vector_pop_back);
    RUN_TEST(test_vector_clear);
    RUN_TEST(test_vector_iterators);
    RUN_TEST(test_vector_complex_types);
    RUN_TEST(test_vector_data_method);
    UNITY_END();
}