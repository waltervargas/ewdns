#include <check.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
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

START_TEST (test_create_listener_used_port)
{
    int fd = create_udp_listener(5301);
    int fd2 = create_udp_listener(5301);
    ck_assert_int_eq(fd2, -1);
    ck_assert_str_eq(strerror(errno),"Address already in use");
    ck_assert_int_eq(close(fd), 0);
}
END_TEST


START_TEST(udp_conn)
{
    int port = 5302;
    int fd = create_udp_listener(port);
    ck_assert_msg(fd != -1, "unable to create UDP listener");

    // create a client socket to send a test message
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    const char *msg = "test";
    sendto(client_fd, msg, strlen(msg), 0,
           (struct sockaddr*)&client_addr, sizeof(client_addr));

    // receive the test message in the listener socket
    char buff[strlen(msg)];
    recvfrom(fd, buff, sizeof(buff), 0, NULL, NULL);

    // check that what we read from the fd into the buff is the same
    // message that we sent.
    ck_assert_str_eq(buff, msg);

    close(fd);
    close(client_fd);
}

Suite* suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("udp");
    tc = tcase_create("UDP");

    tcase_add_test(tc, test_create_udp_listener);
    tcase_add_test(tc, test_create_listener_used_port);
    tcase_add_test(tc, udp_conn);

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
