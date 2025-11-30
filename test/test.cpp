#include <unity.h>

void test_array_basic();
void test_array_access();
void test_array_fill();
void test_array_args_init();
void test_array_make_array();
void test_array_iterator();

void test_queue_inc_cap();
void test_queue_push_back();
void test_queue_push_front();
void test_queue_pop_front();

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_array_basic);
    RUN_TEST(test_array_access);
    RUN_TEST(test_array_fill);
    RUN_TEST(test_array_args_init);
    RUN_TEST(test_array_iterator);
    RUN_TEST(test_array_make_array);

    RUN_TEST(test_queue_inc_cap);
    RUN_TEST(test_queue_push_back);
    RUN_TEST(test_queue_push_front);
    RUN_TEST(test_queue_pop_front);
    UNITY_END();
}
