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
void test_queue_basic_fifo();
void test_queue_large_scale();
void test_queue_mixed_operations();
void test_queue_edge_cases();
void test_queue_copy_and_assignment();

void test_vector_default_construction();
void test_vector_copy_construction();
void test_vector_move_construction();
void test_vector_copy_assignment();
void test_vector_move_assignment();
void test_vector_parameter_pack_construction();
void test_vector_push_emplace_back();
void test_vector_element_access();
void test_vector_pop_back();
void test_vector_clear();
void test_vector_iterators();
void test_vector_complex_types();
void test_vector_data_method();

void test_basic_new_delete();
void test_array_new_delete();
void test_placement_new();
void test_nothrow_new();
void test_multiple_allocations();
void test_zero_size_allocation();


void test_string_constructors();
void test_string_assignment();
void test_string_access();
void test_string_capacity();
void test_string_modifiers();
void test_string_concatenation();
void test_string_iterators();
void test_string_edge_cases();
void test_string_data_methods();

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_array_basic);
    RUN_TEST(test_array_access);
    RUN_TEST(test_array_fill);
    RUN_TEST(test_array_args_init);
    RUN_TEST(test_array_iterator);
    RUN_TEST(test_array_make_array);

    RUN_TEST(test_queue_push_back);
    RUN_TEST(test_queue_push_front);
    RUN_TEST(test_queue_pop_front);
    RUN_TEST(test_queue_inc_cap);
    RUN_TEST(test_queue_basic_fifo);
    RUN_TEST(test_queue_mixed_operations);
    RUN_TEST(test_queue_edge_cases);
    RUN_TEST(test_queue_copy_and_assignment);
    RUN_TEST(test_queue_large_scale);

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

    RUN_TEST(test_string_constructors);
    RUN_TEST(test_string_assignment);
    RUN_TEST(test_string_access);
    RUN_TEST(test_string_capacity);
    RUN_TEST(test_string_modifiers);
    RUN_TEST(test_string_concatenation);
    RUN_TEST(test_string_iterators);
    RUN_TEST(test_string_edge_cases);
    RUN_TEST(test_string_data_methods);

    RUN_TEST(test_basic_new_delete);
    RUN_TEST(test_array_new_delete);
    RUN_TEST(test_placement_new);
    RUN_TEST(test_nothrow_new);
    RUN_TEST(test_multiple_allocations);
    RUN_TEST(test_zero_size_allocation);

    UNITY_END();
}
