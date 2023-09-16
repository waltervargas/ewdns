#include <check.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "../src/udp.h"

char *generate_random_string(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *str = malloc(length + 1);
    if (str == NULL) {
        return NULL; // Allocation failed
    }

    srand(time(0));
    for (size_t i = 0; i < length; ++i) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }
    str[length] = '\0'; // Null-terminate the string

    return str;
}


int send_via_udp(int port, const char* buf, size_t buflen){
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int client_fd = socket(AF_INET, SOCK_DGRAM, 0);

    int len;
    if ((len = sendto(client_fd, buf, buflen, 0,
                (struct sockaddr*)&client_addr, sizeof(client_addr))) == -1){
        return -1;
    }

    return client_fd;
}

START_TEST(test_udp_echo)
{
                                        // max udp payload is 65507 Bytes
                                        // flip 0 to 1 to make this test fail
    char *want = generate_random_string(65507 + 0);
    int want_l = strlen(want);
    char got[want_l];

    int port = 5299;
    int fd = create_udp_listener(port);
    ck_assert_int_ge(fd, 0);

    int client_fd;
    client_fd = send_via_udp(port, want, want_l);
    ck_assert_msg(client_fd != -1, "unable to send message via udp");

    // echo the message
    udp_echo(fd, got, want_l);

    // receive the test message in the listener socket
    recvfrom(client_fd, got, want_l, 0, NULL, NULL);

    //check
    got[want_l] = '\0';
    ck_assert_str_eq(want, got);
    free(want);
}

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

    char* want = generate_random_string(10000);
    int want_s = strlen(want);
    char got[want_s];

    int client_fd = send_via_udp(port, want, want_s);
    ck_assert_msg(client_fd != -1, "unable to send udp message");

    // receive the test message in the listener socket
    recvfrom(fd, got, sizeof(got), 0, NULL, NULL);

    // check that what we read from the fd into the buff is the same
    // message that we sent.
    got[want_s] = '\0';
    ck_assert_str_eq(want, got);

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
    tcase_add_test(tc, test_udp_echo);

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
