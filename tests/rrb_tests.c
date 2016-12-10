#include <check.h>
#include <vector.h>
#include <stdlib.h>

START_TEST(avl_create)
{
    imc_vector_t* avl = imc_avl_new();
    ck_assert_ptr_ne(avl, NULL);
    avl->unref(avl);
}
END_TEST

Suite* avl_suite(void) {
    Suite* suite; TCase* tc_core;
    suite = suite_create("AVL");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, avl_create);
    suite_add_tcase(suite, tc_core);

    return suite;
}

int main(void) {
    Suite* suite = avl_suite();
    SRunner* suite_runner = srunner_create(suite);

    srunner_run_all(suite_runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return (number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
