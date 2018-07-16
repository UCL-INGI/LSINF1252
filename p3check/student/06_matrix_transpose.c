#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_transpose_test0:1 => [Matrice creuse: transposée d'une matrice nulle]
void sp_matrix_transpose_test0() {
    struct sp_matrix *m1;
    m1 = sp_get_empty_matrix(6, 4);
    assert(m1);
    assert(sp_check_coherency(m1));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m2 = sp_matrix_transpose(m1);
        CU_ASSERT_PTR_NOT_NULL(m2);
        if(! sp_check_coherency(m2)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m2->nlines, m1->ncols);
            CU_ASSERT_EQUAL(m2->ncols, m1->nlines);
            CU_ASSERT_PTR_NULL(m2->lines);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_transpose_test1:1 => [Matrice creuse: transposée d'une matrice avec éléments]
void sp_matrix_transpose_test1() {
    struct sp_matrix *m1;
    struct line *l;
    struct elem *e;
    m1 = sp_get_test_matrix1();
    assert(m1);
    assert(sp_check_coherency(m1));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m2 = sp_matrix_transpose(m1);
        CU_ASSERT_PTR_NOT_NULL(m2);
        if(! sp_check_coherency(m2)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m2->nlines, m1->ncols);
            CU_ASSERT_EQUAL(m2->ncols, m1->nlines);
            
            l = fetch_line(m2, 0);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 4.0, m2->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 4);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 7.0, m2->precision);
            e = fetch_elem(l, 5);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m2, 1);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 1.0, m2->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 5.0, m2->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 4);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 8.0, m2->precision);
            e = fetch_elem(l, 5);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m2, 2);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 2.0, m2->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 6.0, m2->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 4);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 5);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m2, 3);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 3.0, m2->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 4);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 9.0, m2->precision);
            e = fetch_elem(l, 5);
            CU_ASSERT_PTR_NULL(e); 
            
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_transpose_test2:1 => [Matrice creuse: transposée d'une matrice avec une colonne nulle]
void sp_matrix_transpose_test2() {
    struct sp_matrix *m1;
    m1 = sp_get_test_matrix4();
    assert(m1);
    assert(sp_check_coherency(m1));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m2 = sp_matrix_transpose(m1);
        CU_ASSERT_PTR_NOT_NULL(m2);
        if(! sp_check_coherency(m2)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m2->nlines, m1->ncols);
            CU_ASSERT_EQUAL(m2->ncols, m1->nlines);
            struct line *l = fetch_line(m2, 0);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m2, 1);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 1.0, m2->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_transpose_test3:1 => [Matrice creuse: transposée d'une matrice et échec de malloc]
void sp_matrix_transpose_test3() {
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
        struct sp_matrix *m2 = sp_matrix_transpose(m1);
        CU_ASSERT_PTR_NULL(m2);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}

// @p1archive:matrix_transpose_test0:1 => [Matricetableau: transposée d'une matrice]
void matrix_transpose_test0() {
    struct matrix *m1;
    m1 = get_test_matrix1();
    assert(m1);

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m2 = matrix_transpose(m1);
        CU_ASSERT_PTR_NOT_NULL(m2);
        CU_ASSERT_EQUAL(m2->nlines, m1->ncols);
        CU_ASSERT_EQUAL(m2->ncols, m1->nlines);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[1][2], 5.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL(m2->elems[3][4], 9.0, 0.001);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:matrix_transpose_test1:1 => [Matrice tableau: transposée d'une matrice et échec de malloc]
void matrix_transpose_test1() {
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
        struct matrix *m2 = matrix_transpose(m1);
        CU_ASSERT_PTR_NULL(m2);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}
