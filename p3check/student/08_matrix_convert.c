#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_to_matrix_test0:1 => [Conversion d'une matrice creuse en matrice tableau]
void sp_matrix_to_matrix_test0() {
    struct sp_matrix *m1;
    m1 = sp_get_test_matrix1();
    assert(m1);
    assert(sp_check_coherency(m1));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m2 = sp_matrix_to_matrix(m1);
        CU_ASSERT_PTR_NOT_NULL(m2);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[1][1], 1.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[1][2], 2.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[1][3], 3.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[2][0], 4.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[2][1], 5.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[2][2], 6.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[4][0], 7.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[4][1], 8.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[4][3], 9.0, 0.001);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:matrix_to_sp_matrix_test0:1 => [Conversion d'une matrice tableau en matrice creuse]
void matrix_to_sp_matrix_test0() {
    struct matrix *m1;
    struct line *l;
    struct elem *e;
    m1 = get_test_matrix1();
    assert(m1);

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m2 = matrix_to_sp_matrix(m1, 0.001);
        CU_ASSERT_PTR_NOT_NULL(m2);
        if(! sp_check_coherency(m2)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_DOUBLE_EQUAL(m2->precision, 0.001, 0);
            
            l = fetch_line(m2, 0);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m2, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 1.0, m2->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 2.0, m2->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 3.0, m2->precision);
            
            l = fetch_line(m2, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 4.0, m2->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 5.0, m2->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 6.0, m2->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m2, 3);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m2, 4);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 7.0, m2->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 8.0, m2->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 9.0, m2->precision);
            
            l = fetch_line(m2, 5);
            CU_ASSERT_PTR_NULL(l);
            
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_to_matrix_test1:1 => [Conversion d'une matrice creuse en matrice tableau et erreur malloc]
void sp_matrix_to_matrix_test1() {
    struct sp_matrix *m1;
    m1 = sp_get_test_matrix1();
    assert(m1);
    assert(sp_check_coherency(m1));

    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m2 = sp_matrix_to_matrix(m1);
        CU_ASSERT_PTR_NULL(m2);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}

// @p1archive:matrix_to_sp_matrix_test1:1 => [Conversion d'une matrice tableau en matrice creuse et erreur malloc]
void matrix_to_sp_matrix_test1() {
    struct matrix *m1;
    m1 = get_test_matrix1();
    assert(m1);

    start_timer();
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m2 = matrix_to_sp_matrix(m1, 0.001);
        CU_ASSERT_PTR_NULL(m2);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}