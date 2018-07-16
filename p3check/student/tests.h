#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<setjmp.h>
#include<signal.h>
#include<errno.h>
#include<sys/time.h>
#include<malloc.h>
#include <assert.h>
#define _XOPEN_SOURCE 700
#include <sys/resource.h>

#include<CUnit/CUnit.h>
#include<CUnit/Basic.h>
#include<CUnit/Automated.h>

#include "matrix.h"
#include "libwrap/wrap.h"

extern struct wrap_stats_t stats;
extern struct wrap_monitor_t monitored;
extern struct wrap_fail_t failures;
extern struct wrap_log_t logs;
extern sigjmp_buf segv;
extern struct itimerval it_val;

void start_timer();
void stop_timer();

void sp_matrix_init_test0();
void sp_matrix_init_test1();
void matrix_init_test0();
void matrix_init_test1();

void sp_matrix_free_test0();
void matrix_free_test0();
void sp_matrix_free_test1();
void matrix_free_test1();

void sp_matrix_get_test0();
void sp_matrix_get_test1();
void sp_matrix_get_test2();
void sp_matrix_get_test3();
void sp_matrix_get_test4();
void matrix_get_test0();

void sp_matrix_set_test0();
void sp_matrix_set_test1();
void sp_matrix_set_test2();
void sp_matrix_set_test3();
void sp_matrix_set_test4();
void sp_matrix_set_test5();
void sp_matrix_set_test6();
void sp_matrix_set_test7();
void sp_matrix_set_test8();
void sp_matrix_set_test9();
void sp_matrix_set_test10();
void sp_matrix_set_test11();
void sp_matrix_set_test12();
void sp_matrix_set_test13();
void sp_matrix_set_test14();
void sp_matrix_set_test15();
void matrix_set_test0();

void sp_matrix_add_test0();
void sp_matrix_add_test1();
void sp_matrix_add_test2();
void sp_matrix_add_test3();
void sp_matrix_add_test4();
void sp_matrix_add_test5();
void sp_matrix_add_test6();
void sp_matrix_add_test7();
void sp_matrix_add_test8();
void sp_matrix_add_test9();
void matrix_add_test0();
void matrix_add_test1();

void sp_matrix_transpose_test0();
void sp_matrix_transpose_test1();
void sp_matrix_transpose_test2();
void sp_matrix_transpose_test3();
void matrix_transpose_test0();
void matrix_transpose_test1();

void sp_matrix_mult_test0();
void sp_matrix_mult_test1();
void sp_matrix_mult_test2();
void sp_matrix_mult_test3();
void sp_matrix_mult_test4();
void matrix_mult_test0();
void matrix_mult_test1();

void sp_matrix_to_matrix_test0();
void sp_matrix_to_matrix_test1();
void matrix_to_sp_matrix_test0();
void matrix_to_sp_matrix_test1();
    

struct elem *fetch_elem(struct line *l, unsigned int j);
struct line *fetch_line(struct sp_matrix *m, unsigned int i);

struct sp_matrix *sp_get_empty_matrix(int nlines, int ncols);
struct sp_matrix *sp_get_test_matrix1();
struct sp_matrix *sp_get_test_matrix2();
struct sp_matrix *sp_get_test_matrix3();
struct sp_matrix *sp_get_test_matrix4();
struct sp_matrix *sp_get_test_matrix5();

struct matrix *get_empty_matrix(int nlines, int ncols);
struct matrix *get_test_matrix1();
struct matrix *get_test_matrix2();
struct matrix *get_test_matrix3();
struct matrix *get_test_matrix4();
struct matrix *get_test_matrix5();
    
bool check_coherency(struct matrix *m);
bool sp_check_coherency(struct sp_matrix *m);