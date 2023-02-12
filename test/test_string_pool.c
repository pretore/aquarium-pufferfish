#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <errno.h>
#include <sea-turtle.h>
#include <triggerfish.h>
#include <pufferfish.h>

#include <test/cmocka.h>

static void check_invalidate_error_on_object_is_null(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    assert_false(pufferfish_string_pool_invalidate(NULL));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_init_error_on_object_is_null(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    assert_false(pufferfish_string_pool_init(NULL));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_init_error_on_memory_allocation_failed(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    struct pufferfish_string_pool object;
    pthread_rwlock_init_is_overridden = true;
    will_return(cmocka_test_pthread_rwlock_init, ENOMEM);
    assert_false(pufferfish_string_pool_init(&object));
    pthread_rwlock_init_is_overridden = false;
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_init(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    struct pufferfish_string_pool object;
    assert_true(pufferfish_string_pool_init(&object));
    assert_true(pufferfish_string_pool_invalidate(&object));
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_get_error_on_object_is_null(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    assert_false(pufferfish_string_pool_get(NULL, (void *) 1, (void *) 1));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_get_error_on_string_is_null(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    assert_false(pufferfish_string_pool_get((void *) 1, NULL, (void *) 1));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_STRING_IS_NULL,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_get_error_on_out_is_null(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    assert_false(pufferfish_string_pool_get((void *) 1, (void *) 1, NULL));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_OUT_IS_NULL,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_get_error_on_concurrent_limit_reached(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    struct pufferfish_string_pool object = {};
    pthread_rwlock_rdlock_is_overridden = true;
    will_return(cmocka_test_pthread_rwlock_rdlock, EAGAIN);
    assert_false(pufferfish_string_pool_get(&object, (void *) 1, (void *) 1));
    pthread_rwlock_rdlock_is_overridden = false;
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_CONCURRENCY_LIMIT_REACHED,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_get(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    struct pufferfish_string_pool object;
    assert_true(pufferfish_string_pool_init(&object));
    const char chars[] = u8"get";
    size_t count;
    struct sea_turtle_string string;
    assert_true(sea_turtle_string_init(&string, chars, sizeof(chars), &count));
    struct triggerfish_strong *out;
    assert_true(pufferfish_string_pool_get(&object, &string, &out));
    assert_true(triggerfish_strong_count(out, &count));
    assert_int_equal(count, 1);
    struct sea_turtle_string *str;
    assert_true(triggerfish_strong_instance(out, (void **) &str));
    assert_ptr_not_equal(&string, str);
    assert_int_equal(sea_turtle_string_compare(&string, str), 0);
    assert_true(triggerfish_strong_release(out));
    assert_true(sea_turtle_string_invalidate(&string));
    assert_true(pufferfish_string_pool_invalidate(&object));
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_get_error_on_memory_allocation_failed(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    struct pufferfish_string_pool object;
    assert_true(pufferfish_string_pool_init(&object));
    const char chars[] = u8"get";
    size_t count;
    struct sea_turtle_string string;
    assert_true(sea_turtle_string_init(&string, chars, sizeof(chars), &count));
    struct triggerfish_strong *out;
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden
            = posix_memalign_is_overridden = true;
    assert_false(pufferfish_string_pool_get(&object, &string, &out));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED,
                     pufferfish_error);
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden
            = posix_memalign_is_overridden = false;
    assert_true(sea_turtle_string_invalidate(&string));
    assert_true(pufferfish_string_pool_invalidate(&object));
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_shrink_error_on_object_is_null(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    assert_false(pufferfish_string_pool_shrink(NULL));
    assert_int_equal(PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL,
                     pufferfish_error);
    seahorse_error = SEAHORSE_ERROR_NONE;
}

static void check_shrink(void **state) {
    seahorse_error = SEAHORSE_ERROR_NONE;
    struct pufferfish_string_pool object;
    assert_true(pufferfish_string_pool_init(&object));
    const char chars[] = u8"shrink";
    size_t count;
    struct sea_turtle_string string;
    assert_true(sea_turtle_string_init(&string, chars, sizeof(chars), &count));
    struct triggerfish_strong *out;
    assert_true(pufferfish_string_pool_get(&object, &string, &out));
    assert_true(seahorse_red_black_tree_map_s_wr_count(&object.map, &count));
    assert_int_equal(count, 1);
    assert_true(pufferfish_string_pool_shrink(&object));
    assert_true(triggerfish_strong_release(out));
    assert_true(seahorse_red_black_tree_map_s_wr_count(&object.map, &count));
    assert_int_equal(count, 1);
    assert_true(pufferfish_string_pool_shrink(&object));
    assert_true(seahorse_red_black_tree_map_s_wr_count(&object.map, &count));
    assert_int_equal(count, 0);
    assert_true(sea_turtle_string_invalidate(&string));
    assert_true(pufferfish_string_pool_invalidate(&object));
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_memory_allocation_failed),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_get_error_on_object_is_null),
            cmocka_unit_test(check_get_error_on_string_is_null),
            cmocka_unit_test(check_get_error_on_out_is_null),
            cmocka_unit_test(check_get_error_on_concurrent_limit_reached),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_get_error_on_memory_allocation_failed),
            cmocka_unit_test(check_shrink_error_on_object_is_null),
            cmocka_unit_test(check_shrink),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
