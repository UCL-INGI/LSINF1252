#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_mult_test0:1 => [Matrice creuse: multiplication de deux matrices nulles]
void sp_matrix_mult_test0() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_empty_matrix(6, 4);
    m2 = sp_get_empty_matrix(4, 6);
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_mult(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            CU_ASSERT_PTR_NULL(m1->lines);
            CU_ASSERT_PTR_NULL(m2->lines);
            CU_ASSERT_PTR_NULL(m3->lines);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_mult_test1:1 => [Matrice creuse: multiplication d'une matrice avec une matrice nulle]
void sp_matrix_mult_test1() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_empty_matrix(4, 6);
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_mult(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            CU_ASSERT_PTR_NULL(m2->lines);
            CU_ASSERT_PTR_NULL(m3->lines);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_mult_test2:1 => [Matrice creuse: erreur lors de la multiplication de matrices de tailles incompatibles]
void sp_matrix_mult_test2() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_empty_matrix(4,6);
    m2 = sp_get_empty_matrix(4,6);
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_mult(m1, m2);
        CU_ASSERT_PTR_NULL(m3);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_mult_test3:1 => [Matrice creuse: multiplication de deux matrices quelconques]
void sp_matrix_mult_test3() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix5();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_mult(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            
            struct line *l = fetch_line(m3, 4);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 4);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 194.0, m3->precision);
            
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_mult_test4:1 => [Matrice creuse: multiplication de deux matrices et échec de malloc]
void sp_matrix_mult_test4() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix5();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_mult(m1, m2);
        CU_ASSERT_PTR_NULL(m3);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}

// @p1archive:matrix_mult_test0:1 => [Matrice tableau: multiplication de deux matrices quelconques]
void matrix_mult_test0() {
    struct matrix *m1, *m2;
    m1 = get_test_matrix1();
    m2 = get_test_matrix5();
    assert(m1);
    assert(m2);

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m3 = matrix_mult(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
        CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
        CU_ASSERT_DOUBLE_EQUAL(m3->elems[4][4], 194.0, 0.001);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:matrix_mult_test1:1 => [Matrice tableau: multiplication de deux matrices et échec de malloc]
void matrix_mult_test1() {
    struct matrix *m1, *m2;
    m1 = get_test_matrix1();
    m2 = get_test_matrix5();
    assert(m1);
    assert(m2);
    

    start_timer();
    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m3 = matrix_mult(m1, m2);
        CU_ASSERT_PTR_NULL(m3);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}