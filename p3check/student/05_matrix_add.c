#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_add_test0:1 => [Matrice creuse: addition de deux matrices nulles]
void sp_matrix_add_test0() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_empty_matrix(6, 4);
    m2 = sp_get_empty_matrix(6, 4);
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
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

// @p1archive:sp_matrix_add_test1:1 => [Matrice creuse: addition d'une matrice avec une matrice nulle]
void sp_matrix_add_test1() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_empty_matrix(6, 4);
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            CU_ASSERT_PTR_NULL(m2->lines);

            struct line *l = fetch_line(m3, 0);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m3, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 1.0, m3->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 2.0, m3->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 3.0, m3->precision);
            
            l = fetch_line(m3, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 4.0, m3->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 5.0, m3->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 6.0, m3->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m3, 3);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m3, 4);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 7.0, m3->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 8.0, m3->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 9.0, m3->precision);
            
            l = fetch_line(m3, 5);
            CU_ASSERT_PTR_NULL(l);
            
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test2:1 => [Matrice creuse: addition commutative]
void sp_matrix_add_test2() {
    struct sp_matrix *m1, *m2;
    struct line *l;
    struct elem *e;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_empty_matrix(6, 4);
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        struct sp_matrix *m4 = sp_matrix_add(m2, m1);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            CU_ASSERT_PTR_NULL(m2->lines);

            l = fetch_line(m3, 0);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m3, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 1.0, m3->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 2.0, m3->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 3.0, m3->precision);
            
            l = fetch_line(m3, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 4.0, m3->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 5.0, m3->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 6.0, m3->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m3, 3);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m3, 4);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 7.0, m3->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 8.0, m3->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 9.0, m3->precision);
            
            l = fetch_line(m3, 5);
            CU_ASSERT_PTR_NULL(l);
            
            // M4
            
            CU_ASSERT_EQUAL(m4->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m4->ncols, m2->ncols);

            l = fetch_line(m4, 0);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m4, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NULL(e);
            
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 1.0, m4->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 2.0, m4->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 3.0, m4->precision);
            
            l = fetch_line(m4, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 4.0, m4->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 5.0, m4->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 6.0, m4->precision);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NULL(e);
            
            l = fetch_line(m4, 3);
            CU_ASSERT_PTR_NULL(l);
            
            l = fetch_line(m4, 4);
            CU_ASSERT_PTR_NOT_NULL(l);
            e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 7.0, m4->precision);
            e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 8.0, m4->precision);
            e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NULL(e);
            e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 9.0, m4->precision);
            
            l = fetch_line(m4, 5);
            CU_ASSERT_PTR_NULL(l);
            
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test3:1 => [Matrice creuse: erreur lors de l'addition de matrices de différentes tailles]
void sp_matrix_add_test3() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix4();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NULL(m3);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test4:1 => [Matrice creuse: addition de deux matrices et annulation d'éléments]
void sp_matrix_add_test4() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix2();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            
            struct line *l = fetch_line(m3, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NULL(e);
            
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test5:1 => [Matrice creuse: addition de deux matrices et annulation d'une ligne]
void sp_matrix_add_test5() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix2();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            
            struct line *l = fetch_line(m3, 1);
            CU_ASSERT_PTR_NULL(l);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test6:1 => [Matrice creuse: addition de deux matrices opposées]
void sp_matrix_add_test6() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix3();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            
            CU_ASSERT_PTR_NULL(m3->lines);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test7:1 => [Matrice creuse: addition de deux matrices et création d'un élément]
void sp_matrix_add_test7() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix2();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            
            struct line *l = fetch_line(m3, 0);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 9.0, m3->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test8:1 => [Matrice creuse: addition de deux matrices et addition des éléments]
void sp_matrix_add_test8() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix2();
    assert(m1);
    assert(sp_check_coherency(m1));
    assert(m2);
    assert(sp_check_coherency(m2));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        if(! sp_check_coherency(m3)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
            CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
            
            struct line *l = fetch_line(m3, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 8.0, m3->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_add_test9:1 => [Matrice creuse: addition de deux matrices et échec de malloc]
void sp_matrix_add_test9() {
    struct sp_matrix *m1, *m2;
    m1 = sp_get_test_matrix1();
    m2 = sp_get_test_matrix2();
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
        struct sp_matrix *m3 = sp_matrix_add(m1, m2);
        CU_ASSERT_PTR_NULL(m3);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}

// @p1archive:matrix_add_test0:1 => [Matrice tableau: addition de deux matrices et addition des éléments]
void matrix_add_test0() {
    struct matrix *m1, *m2;
    m1 = get_test_matrix1();
    m2 = get_test_matrix2();
    assert(m1);
    assert(m2);

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        struct matrix *m3 = matrix_add(m1, m2);
        CU_ASSERT_PTR_NOT_NULL(m3);
        CU_ASSERT_EQUAL(m3->nlines, m1->nlines);
        CU_ASSERT_EQUAL(m3->ncols, m2->ncols);
        CU_ASSERT_DOUBLE_EQUAL(m3->elems[2][0], 8.0, 0.001);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:matrix_add_test1:1 => [Matrice tableau: addition de deux matrices et échec de malloc]
void matrix_add_test1() {
    struct matrix *m1, *m2;
    m1 = get_test_matrix1();
    m2 = get_test_matrix2();
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
        struct matrix *m3 = matrix_add(m1, m2);
        CU_ASSERT_PTR_NULL(m3);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    monitored.malloc=false;
    monitored.calloc=false;
    stop_timer();
}