#include <check.h>
#include <vector.h>
#include <stdlib.h>

START_TEST(rrb_create)
{
    imc_vector_t* rrb = imc_rrb_new();

    // Check for the creation.
    ck_assert_ptr_ne(rrb, NULL);

    rrb->unref(rrb);
}
END_TEST

START_TEST(rrb_push)
{
    imc_data_t* data   = malloc(sizeof *data);
    imc_vector_t* rrb1 = imc_rrb_new();

    // Check for the new AVL.
    imc_vector_t* rrb2 = rrb1->push(rrb1, data);
    ck_assert_ptr_ne(rrb2, NULL);

    // Check for the value.
    imc_data_t* lookup = rrb2->lookup(rrb2, 1);
    ck_assert_ptr_eq(data, lookup);

    // Clean a little bit.
    rrb1->unref(rrb1);
    rrb2->unref(rrb2);
}
END_TEST

Suite* rrb_suite(void) {
    Suite* suite   = suite_create("AVL");
    TCase* tc_core = tcase_create("Core");

    tcase_add_test(tc_core, rrb_create);
    tcase_add_test(tc_core, rrb_push);
    suite_add_tcase(suite, tc_core);

    return suite;
}

int main(void) {
    Suite*   suite = rrb_suite();
    SRunner* suite_runner = srunner_create(suite);

    srunner_run_all(suite_runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return (number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
