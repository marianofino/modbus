/**/
#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <check.h>
#include "../src/cli_helper.h"

char * argv[5];
int argc = 5;
cli_arguments_t options;

void setup(void)
{
  argv[0] = (char *)malloc(sizeof(char) * 7);
  strcpy(argv[0], "modbus");

  argv[1] = (char *)malloc(sizeof(char) * 3);
  strcpy(argv[1], "-u");

  argv[2] = (char *)malloc(sizeof(char) * 12);
  strcpy(argv[2], "10.10.10.10");

  argv[3] = (char *)malloc(sizeof(char) * 3);
  strcpy(argv[3], "-f");

  argv[4] = (char *)malloc(sizeof(char) * 3);
  strcpy(argv[4], "02");
}

void teardown(void)
{
  free(argv[0]);
  free(argv[1]);
  free(argv[2]);
  free(argv[3]);
  free(argv[4]);
}

START_TEST(test_args_parsed)
{
    int result = parseArguments(argc, argv, &options);
    ck_assert_int_eq(result, 0);
    ck_assert_str_eq(options.url, "10.10.10.10");
    ck_assert_int_eq(options.function, 2);
}
END_TEST

Suite * cli_helper_suite(void) {

    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    s = suite_create("Cli Helper");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_args_parsed);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {

    int number_failed;
    Suite *s;
    SRunner *sr;

    s = cli_helper_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
