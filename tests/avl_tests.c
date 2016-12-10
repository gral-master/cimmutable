#include <check.h>
#include <vector.h>
#include <stdlib.h>

START_TEST(avl_create)
{
    imc_vector_t* avl = imc_avl_new();

    // Check for the creation.
    ck_assert_ptr_ne(avl, NULL);

    avl->unref(avl);
}
END_TEST

START_TEST(avl_push)
{
    imc_data_t* data   = malloc(sizeof *data);
    imc_vector_t* avl1 = imc_avl_new();

    // Check for the new AVL.
    imc_vector_t* avl2 = avl1->push(avl1, data);
    ck_assert_ptr_ne(avl2, NULL);

    // Check for the value.
    imc_data_t* lookup = avl2->lookup(avl2, 1);
    ck_assert_ptr_eq(data, lookup);

    // Cleana little bit.
    avl1->unref(avl1);
    avl2->unref(avl2);
}
END_TEST

Suite* avl_suite(void) {
    Suite* suite   = suite_create("AVL");
    TCase* tc_core = tcase_create("Core");

    tcase_add_test(tc_core, avl_create);
    tcase_add_test(tc_core, avl_push);
    suite_add_tcase(suite, tc_core);

    return suite;
}

int main(void) {
    Suite*   suite = avl_suite();
    SRunner* suite_runner = srunner_create(suite);

    srunner_run_all(suite_runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return (number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
