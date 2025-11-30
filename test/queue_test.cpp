#include <unity.h>

#include <queue.hpp>

using msd::queue;

void test_queue_push_back() {
    queue<int> q;
    for (int i = 0; i <= 127; i++) q.push_back(i);
    for (int i = 127; i >= 0; i--) {
        TEST_ASSERT_EQUAL(i, q.back());
        q.pop_back();
    }
    TEST_ASSERT_TRUE(q.empty());
}

void test_queue_push_front() {
    queue<int> q;
    for (int i = 0; i <= 127; i++) q.push_front(i);
    for (int i = 127; i >= 0; i--) {
        TEST_ASSERT_EQUAL(i, q.front());
        q.pop_front();
    }
}

void test_queue_pop_front() {
    queue<int> q;
    for (int i = 0; i <= 200; i++) q.push_back(i);
    for (int i = 0; i <= 200; i++) {
        TEST_ASSERT_EQUAL(0, q.front());
        q.pop_back();
    }
    TEST_ASSERT_TRUE(q.empty());
}

void test_queue_inc_cap() {
    queue<int> q(2);
    for (int i = 0; i <= 127; i++) q.push_back(i);
    for (int i = 0; i <= 127; i++) {
        TEST_ASSERT_EQUAL(i, q.front());
        q.pop_front();
    }
    TEST_ASSERT_EQUAL(128, q.capacity());
    TEST_ASSERT_TRUE(q.empty());
}
