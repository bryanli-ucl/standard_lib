#pragma once

#include <unity.h>

#include <queue>
using msd::queue;

// Test basic push_back and pop_back functionality
void test_queue_push_back() {
    queue<int, 128> q;
    for (int i = 0; i <= 127; i++) q.push_back(i);
    for (int i = 127; i >= 0; i--) {
        TEST_ASSERT_EQUAL(i, q.back());
        q.pop_back();
    }
    TEST_ASSERT_TRUE(q.empty());
}

// Test push_front functionality
void test_queue_push_front() {
    queue<int, 128> q;
    for (int i = 0; i <= 127; i++) q.push_front(i);
    for (int i = 127; i >= 0; i--) {
        TEST_ASSERT_EQUAL(i, q.front());
        q.pop_front();
    }
}

// Test pop_front functionality with push_back
void test_queue_pop_front() {
    queue<int, 256> q;
    for (int i = 0; i <= 200; i++) q.push_back(i);
    for (int i = 0; i <= 200; i++) {
        TEST_ASSERT_EQUAL(i, q.front());
        q.pop_front();
    }
    TEST_ASSERT_TRUE(q.empty());
}

// Test capacity increase functionality
void test_queue_inc_cap() {
    queue<int, 128> q;
    for (int i = 0; i <= 127; i++) q.push_back(i);
    for (int i = 0; i <= 127; i++) {
        TEST_ASSERT_EQUAL(i, q.front());
        q.pop_front();
    }
    TEST_ASSERT_EQUAL(128, q.capacity());
    TEST_ASSERT_TRUE(q.empty());
}

// Test basic FIFO (First-In-First-Out) behavior
void test_queue_basic_fifo() {
    queue<int> q;

    // Test empty queue
    TEST_ASSERT_TRUE(q.empty());
    TEST_ASSERT_EQUAL(0, q.size());

    // Add elements
    q.push_back(1);
    q.push_back(2);
    q.push_back(3);

    // Test size and non-empty
    TEST_ASSERT_FALSE(q.empty());
    TEST_ASSERT_EQUAL(3, q.size());

    // Test FIFO order
    TEST_ASSERT_EQUAL(1, q.front());
    TEST_ASSERT_EQUAL(3, q.back());

    q.pop_front();
    TEST_ASSERT_EQUAL(2, q.front());
    TEST_ASSERT_EQUAL(3, q.back());

    q.pop_front();
    TEST_ASSERT_EQUAL(3, q.front());
    TEST_ASSERT_EQUAL(3, q.back());

    q.pop_front();
    TEST_ASSERT_TRUE(q.empty());
}

// Test mixed operations (push front/back, pop front/back)
void test_queue_mixed_operations() {
    queue<int> q;

    // Alternate between front and back pushes
    q.push_back(10);
    q.push_front(5);
    q.push_back(15);
    q.push_front(1);

    TEST_ASSERT_EQUAL(1, q.front());
    TEST_ASSERT_EQUAL(15, q.back());
    TEST_ASSERT_EQUAL(4, q.size());

    // Alternate between front and back pops
    q.pop_front(); // Remove 1
    TEST_ASSERT_EQUAL(5, q.front());
    TEST_ASSERT_EQUAL(15, q.back());

    q.pop_back(); // Remove 15
    TEST_ASSERT_EQUAL(5, q.front());
    TEST_ASSERT_EQUAL(10, q.back());

    q.pop_front(); // Remove 5
    TEST_ASSERT_EQUAL(10, q.front());
    TEST_ASSERT_EQUAL(10, q.back());
}

// Test edge cases and boundary conditions
void test_queue_edge_cases() {
    queue<int> q;

    // Test single element queue
    q.push_back(42);
    TEST_ASSERT_EQUAL(42, q.front());
    TEST_ASSERT_EQUAL(42, q.back());
    TEST_ASSERT_EQUAL(1, q.size());
    TEST_ASSERT_FALSE(q.empty());

    q.pop_front();
    TEST_ASSERT_TRUE(q.empty());

    // Test operations on empty queue (should handle safely)
    q.pop_front(); // Should not crash
    q.pop_back();  // Should not crash

    TEST_ASSERT_TRUE(q.empty());
    TEST_ASSERT_EQUAL(0, q.size());
}

// Test copy constructor and assignment operator
void test_queue_copy_and_assignment() {
    queue<int> original;
    for (int i = 0; i < 5; i++) {
        original.push_back(i * 10);
    }

    // Copy constructor
    queue<int> copy(original);
    TEST_ASSERT_EQUAL(original.size(), copy.size());

    while (!original.empty()) {
        TEST_ASSERT_EQUAL(original.front(), copy.front());
        original.pop_front();
        copy.pop_front();
    }

    // Refill for assignment test
    for (int i = 0; i < 3; i++) {
        original.push_back(i * 5);
    }

    // Assignment operator
    queue<int> assigned;
    assigned = original;

    TEST_ASSERT_EQUAL(original.size(), assigned.size());

    while (!original.empty()) {
        TEST_ASSERT_EQUAL(original.front(), assigned.front());
        original.pop_front();
        assigned.pop_front();
    }
}

// Test large number of elements
void test_queue_large_scale() {
    const int LARGE_NUMBER = 1000;
    queue<int, LARGE_NUMBER> q;

    // Push large number of elements
    for (int i = 0; i < LARGE_NUMBER; i++) {
        q.push_back(i);
    }

    TEST_ASSERT_EQUAL(LARGE_NUMBER, q.size());

    // Verify all elements in order
    for (int i = 0; i < LARGE_NUMBER; i++) {
        TEST_ASSERT_EQUAL(i, q.front());
        q.pop_front();
    }

    TEST_ASSERT_TRUE(q.empty());
}

void test_queue() {
    UNITY_BEGIN();

    RUN_TEST(test_queue_push_back);
    RUN_TEST(test_queue_push_front);
    RUN_TEST(test_queue_pop_front);
    RUN_TEST(test_queue_inc_cap);
    RUN_TEST(test_queue_basic_fifo);
    RUN_TEST(test_queue_mixed_operations);
    RUN_TEST(test_queue_edge_cases);
    RUN_TEST(test_queue_copy_and_assignment);
    RUN_TEST(test_queue_large_scale);

    UNITY_END();
}