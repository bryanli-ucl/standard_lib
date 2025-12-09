#include <unity.h>

#include "test_move.hpp"
#include "test_pair.hpp"
#include "test_queue.hpp"
#include "test_tuple.hpp"
#include "test_type_trait.hpp"
#include "test_vector.hpp"

void test_array_basic();
void test_array_access();
void test_array_fill();
void test_array_args_init();
void test_array_make_array();
void test_array_iterator();

void test_basic_new_delete();
void test_array_new_delete();
void test_placement_new();
void test_nothrow_new();
void test_multiple_allocations();
void test_zero_size_allocation();

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_array_basic);
    RUN_TEST(test_array_access);
    RUN_TEST(test_array_fill);
    RUN_TEST(test_array_args_init);
    RUN_TEST(test_array_iterator);
    RUN_TEST(test_array_make_array);

    RUN_TEST(test_basic_new_delete);
    RUN_TEST(test_array_new_delete);
    RUN_TEST(test_placement_new);
    RUN_TEST(test_nothrow_new);
    RUN_TEST(test_multiple_allocations);
    RUN_TEST(test_zero_size_allocation);

    RUN_TEST(test_remove_reference);
    RUN_TEST(test_move_basic_types);
    RUN_TEST(test_move_custom_class);
    RUN_TEST(test_forward_function);
    RUN_TEST(test_swap_basic_types);
    RUN_TEST(test_swap_custom_class);
    RUN_TEST(test_swap_array);

    UNITY_END();
    test_queue();

    test_vector();
    test_tuple();
    test_type_traits();
    test_pair_basic();
}
