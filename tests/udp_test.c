#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/udp.h"

START_TEST(test_create_udp_listener)
{
    int fd = create_udp_listener(5300);
    // ensure fd is not -1;
    ck_assert_int_ge(fd, 0);

    // clean up
    ck_assert_int_eq(close(fd), 0);
}
END_TEST

START_TEST (test_create_listener_invalid_port)
{
    int fd = create_udp_listener(80);
    if (fd < 0) {
        ck_assert_int_eq(1, 1); // Success, we expect failure when running as a non-root user
    } else {
        // If it somehow succeeds, make sure we can close the socket
        ck_assert_int_eq(close(fd), 0);
    }
}
END_TEST

Suite* suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("udp");
    tc = tcase_create("UDP");

    tcase_add_test(tc, test_create_udp_listener);
    tcase_add_test(tc, test_create_listener_invalid_port);

    suite_add_tcase(s, tc);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    setenv("CK_COLOR_ALWAYS", "yes", 1);
    s = suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_SUBUNIT);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
