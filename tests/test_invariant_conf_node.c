#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "src/conf_io/conf_node.h"

START_TEST(test_buffer_reads_never_exceed_declared_length)
{
    // Invariant: Buffer reads never exceed the declared length
    const char *payloads[] = {
        "normal_name",                    // Valid input
        "very_long_name_that_exceeds_buffer_by_2x_",  // 2x overflow
        "extremely_long_name_that_exceeds_buffer_by_10x_and_causes_overflow_attack_payload_1234567890"  // 10x overflow
    };
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        conf_node_t *node = conf_node_create();
        ck_assert_ptr_nonnull(node);
        
        // This should either safely truncate or reject the input
        conf_node_set_name(node, payloads[i]);
        
        // Verify the name was either truncated to safe length or set to empty
        size_t actual_len = strlen(node->name.small_str);
        ck_assert_msg(actual_len < CONF_NODE_NAME_MAX, 
                     "Buffer overflow detected: name length %zu exceeds max %d", 
                     actual_len, CONF_NODE_NAME_MAX);
        
        conf_node_destroy(node);
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_buffer_reads_never_exceed_declared_length);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}