#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_init_test0:1 => [Initialisation d'une matrice creuse]
void sp_matrix_init_test0() {
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m = sp_matrix_init(0.001, 6, 4);
        CU_ASSERT_NOT_EQUAL(m, NULL);
        
        if (!sp_check_coherency(m))
            CU_FAIL("m is not coherent.")
            
        CU_ASSERT_EQUAL(m->lines, NULL);
        CU_ASSERT_EQUAL(m->nlines, 6);
        CU_ASSERT_EQUAL(m->ncols, 4);
        CU_ASSERT_EQUAL(m->precision, 0.001);
        
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_init_test1:1 => [Retourne NULL en cas d'erreur d'allocation matrice creuse]
void sp_matrix_init_test1() {
    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m = sp_matrix_init(0.001, 6, 4);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
        CU_ASSERT_PTR_NULL(m);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}

// @p1archive:matrix_init_test0:1 => [Initialisation d'une matrice tableau]
void matrix_init_test0() {
    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m = matrix_init(6, 4);
        CU_ASSERT_NOT_EQUAL(m, NULL);
        CU_ASSERT_EQUAL(m->nlines, 6);
        CU_ASSERT_EQUAL(m->ncols, 4);
        CU_ASSERT_PTR_NOT_NULL(m->elems);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)>=1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}

// @p1archive:matrix_init_test1:1 => [Retourne NULL en cas d'erreur d'allocation matrice tableau]
void matrix_init_test1() {
    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m = matrix_init(6, 4);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
        CU_ASSERT_PTR_NULL(m);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}