#include "matrix.h"
#include "tests.h"
#include "libwrap/wrap.h"

int __real_exit(int status);
int __wrap_exit(int status){
    return status;
}

struct wrap_stats_t stats;
struct wrap_monitor_t monitored;
struct wrap_fail_t failures;
struct wrap_log_t logs;
sigjmp_buf segv;
struct itimerval it_val;

static void handler(int sig, siginfo_t *unused, void *unused2)
{
    siglongjmp(segv, 1);
}

void start_timer() {
    it_val.it_value.tv_sec = 2;
    it_val.it_value.tv_usec = 0;
    it_val.it_interval.tv_sec = 0;
    it_val.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &it_val, NULL);
}

void stop_timer() {
    it_val.it_value.tv_sec = 0;
    it_val.it_value.tv_usec = 0;
    it_val.it_interval.tv_sec = 0;
    it_val.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &it_val, NULL);
}

int init_suite1(void) {
    return 0;
}

int clean_suite1(void) {
    return 0;
}

int main(int argc, char** argv) {
    /*Ignore double free*/
    mallopt(M_CHECK_ACTION, 0);

    /* make sure that we catch segmentation faults */
    struct sigaction sa;

    memset(&sa, 0, sizeof(sigaction));
    sigemptyset(&sa.sa_mask);
    static char stack[SIGSTKSZ];
    stack_t ss = {
        .ss_size = SIGSTKSZ,
        .ss_sp = stack,
    };

    sa.sa_flags     = SA_NODEFER|SA_ONSTACK|SA_RESTART;
    sa.sa_sigaction = handler;
    sigaltstack(&ss, 0);
    sigfillset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "sp_matrix_init_test0", sp_matrix_init_test0)) ||
        (NULL == CU_add_test(pSuite, "matrix_init_test0", matrix_init_test0)) ||
        (NULL == CU_add_test(pSuite, "matrix_init_test1", matrix_init_test1)) ||
        (NULL == CU_add_test(pSuite, "sp_matrix_init_test1", sp_matrix_init_test1)) ||
       (NULL == CU_add_test(pSuite, "sp_matrix_free_test0", sp_matrix_free_test0)) ||
       (NULL == CU_add_test(pSuite, "matrix_free_test0", matrix_free_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_free_test1", sp_matrix_free_test1)) ||
       (NULL == CU_add_test(pSuite, "matrix_free_test1", matrix_free_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_get_test0", sp_matrix_get_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_get_test1", sp_matrix_get_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_get_test2", sp_matrix_get_test2)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_get_test3", sp_matrix_get_test3)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_get_test4", sp_matrix_get_test4)) || 
       (NULL == CU_add_test(pSuite, "matrix_get_test0", matrix_get_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test0", sp_matrix_set_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test1", sp_matrix_set_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test2", sp_matrix_set_test2)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test3", sp_matrix_set_test3)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test4", sp_matrix_set_test4)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test5", sp_matrix_set_test5)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test6", sp_matrix_set_test6)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test7", sp_matrix_set_test7)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test8", sp_matrix_set_test8)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test9", sp_matrix_set_test9)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test10", sp_matrix_set_test10)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test11", sp_matrix_set_test11)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test12", sp_matrix_set_test12)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test13", sp_matrix_set_test13)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test14", sp_matrix_set_test14)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_set_test15", sp_matrix_set_test15)) || 
       (NULL == CU_add_test(pSuite, "matrix_set_test0", matrix_set_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test0", sp_matrix_add_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test1", sp_matrix_add_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test2", sp_matrix_add_test2)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test3", sp_matrix_add_test3)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test4", sp_matrix_add_test4)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test5", sp_matrix_add_test5)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test6", sp_matrix_add_test6)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test7", sp_matrix_add_test7)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test8", sp_matrix_add_test8)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_add_test9", sp_matrix_add_test9)) || 
       (NULL == CU_add_test(pSuite, "matrix_add_test0", matrix_add_test0)) || 
       (NULL == CU_add_test(pSuite, "matrix_add_test1", matrix_add_test1))|| 
       (NULL == CU_add_test(pSuite, "sp_matrix_transpose_test0", sp_matrix_transpose_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_transpose_test1", sp_matrix_transpose_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_transpose_test2", sp_matrix_transpose_test2)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_transpose_test3", sp_matrix_transpose_test3)) || 
       (NULL == CU_add_test(pSuite, "matrix_transpose_test0", matrix_transpose_test0)) || 
       (NULL == CU_add_test(pSuite, "matrix_transpose_test1", matrix_transpose_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_mult_test0", sp_matrix_mult_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_mult_test1", sp_matrix_mult_test1))
       || 
       (NULL == CU_add_test(pSuite, "sp_matrix_mult_test2", sp_matrix_mult_test2))
       || 
       (NULL == CU_add_test(pSuite, "sp_matrix_mult_test3", sp_matrix_mult_test3))
       || 
       (NULL == CU_add_test(pSuite, "sp_matrix_mult_test4", sp_matrix_mult_test4)) || 
       (NULL == CU_add_test(pSuite, "matrix_mult_test0", matrix_mult_test0))
       || 
       (NULL == CU_add_test(pSuite, "matrix_mult_test1", matrix_mult_test1)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_to_matrix_test0", sp_matrix_to_matrix_test0)) || 
       (NULL == CU_add_test(pSuite, "sp_matrix_to_matrix_test1", sp_matrix_to_matrix_test1))
       || 
       (NULL == CU_add_test(pSuite, "matrix_to_sp_matrix_test0", matrix_to_sp_matrix_test0)) || 
       (NULL == CU_add_test(pSuite, "matrix_to_sp_matrix_test1", matrix_to_sp_matrix_test1)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    //CU_basic_set_mode(CU_BRM_VERBOSE);
    //CU_basic_run_tests();
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}