#include <stdlib.h>
#include <check.h>
#include <string.h>
#include "../src/utils.h"

START_TEST(test_dns_query_to_hex)
{
    const char* test_data = "abc";
    char hex_str[7];  // 3 characters * 2 hex digits per character + 1 for '\0'

    // Call the function to be tested
    dns_query_to_hex((const unsigned char*)test_data, hex_str, 3);

    // Check the results
    ck_assert_str_eq(hex_str, "616263");  // 61 is 'a' in hex, 62 is 'b', and 63 is 'c'
}
END_TEST

START_TEST(test_format_packet_RFC_style)
{
    // Test data
    const unsigned char test_packet[] = {0x41, 0x42, 0x43, 0x31, 0x32, 0x33};
    int packet_len = sizeof(test_packet);
    char buffer[256]; // Allocate enough space for the formatted output
    memset(buffer, 0, sizeof(buffer));

    const char *expected_result =
        "Byte#   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n"
        "0000   41 42 43 31 32 33                                ABC123..........\n";


    // Call the function
    format_packet_RFC_style(test_packet, packet_len, buffer, sizeof(buffer));

    // Verify the results
    ck_assert_str_eq(buffer, expected_result);
}
END_TEST

Suite* suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("utils");
    tc = tcase_create("Utils");

    tcase_add_test(tc, test_dns_query_to_hex);
    tcase_add_test(tc, test_format_packet_RFC_style);

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
