#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_set_test0:1 => [Matrice creuse: insertion d'un élément dans une matrice nulle]
void sp_matrix_set_test0() {
    struct sp_matrix *m;
    m = sp_get_empty_matrix(6, 4);
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 2, 3, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test1:1 => [Matrice creuse: insertion d'un zéro dans une matrice nulle]
void sp_matrix_set_test1() {
    struct sp_matrix *m;
    m = sp_get_empty_matrix(6, 4);
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 2, 3, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_PTR_NULL(m->lines);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test2:1 => [Matrice creuse: insertion d'un élément dans dans la première ligne de zéros]
void sp_matrix_set_test2() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 0, 0, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 0);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test3:1 => [Matrice creuse: insertion d'un élément dans dans une ligne quelconque de zéros]
void sp_matrix_set_test3() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 3, 0, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 3);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test4:1 => [Matrice creuse: insertion d'un élément dans dans la dernière ligne de zéros]
void sp_matrix_set_test4() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 5, 0, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 5);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test5:1 => [Matrice creuse: insertion d'un zéro dans une ligne de zéros]
void sp_matrix_set_test5() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 0, 0, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 0);
            CU_ASSERT_PTR_NULL(l);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test6:1 => [Matrice creuse: insertion d'un élément au début d'une ligne existante]
void sp_matrix_set_test6() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 0, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 0);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test7:1 => [Matrice creuse: insertion d'un élément au milieu d'une ligne existante]
void sp_matrix_set_test7() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 4, 2, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 4);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 2);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test8:1 => [Matrice creuse: insertion d'un élément à la fin d'une ligne existante]
void sp_matrix_set_test8() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 2, 3, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 2);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 3);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test9:1 => [Matrice creuse: modification d'un élément existant]
void sp_matrix_set_test9() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 1, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NOT_NULL(e);
            CU_ASSERT_DOUBLE_EQUAL(e->value, 42.0, m->precision);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test10:1 => [Matrice creuse: mise à zéro d'un élément]
void sp_matrix_set_test10() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 1, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NULL(e);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test11:1 => [Matrice creuse: mise à zéro d'une ligne existante]
void sp_matrix_set_test11() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 1, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 1, 2, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 1, 3, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 1);
            CU_ASSERT_PTR_NULL(l);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test12:1 => [Matrice creuse: mise à zéro d'une matrice]
void sp_matrix_set_test12() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 1, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 1, 2, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 1, 3, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        struct line *l = fetch_line(m, 1);
        CU_ASSERT_PTR_NULL(l);
        ret = sp_matrix_set(m, 2, 0, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 2, 1, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 2, 2, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        l = fetch_line(m, 2);
        CU_ASSERT_PTR_NULL(l);
        ret = sp_matrix_set(m, 4, 0, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 4, 1, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        ret = sp_matrix_set(m, 4, 3, 0.0);
        CU_ASSERT_EQUAL(ret, 0);
        l = fetch_line(m, 4);
        CU_ASSERT_PTR_NULL(l);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            CU_ASSERT_PTR_NULL(m->lines);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_set_test13:1 => [Matrice creuse: erreur malloc à l'insertion d'un élément]
void sp_matrix_set_test13() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));
    
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 0, 0, 42.0);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==1);
        CU_ASSERT_NOT_EQUAL(ret, 0);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
    monitored.malloc=false;
    monitored.calloc=false;
}

// @p1archive:sp_matrix_set_test14:1 => [Matrice creuse: pas d'erreur malloc au remplacement d'un élément]
void sp_matrix_set_test14() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));
    
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 1, 42.0);
        CU_ASSERT_TRUE((stats.malloc.called+stats.calloc.called)==0);
        CU_ASSERT_EQUAL(ret, 0);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
    monitored.malloc=false;
    monitored.calloc=false;
}

// @p1archive:sp_matrix_set_test15:1 => [Matrice creuse: modification de la valeur d'un élément à une valeur < precision]
void sp_matrix_set_test15() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = sp_matrix_set(m, 1, 1, 0.0001);
        CU_ASSERT_EQUAL(ret, 0);
        if(! sp_check_coherency(m)) {
            CU_FAIL("Non coherent matrix.");
        } else {
            struct line *l = fetch_line(m, 1);
            CU_ASSERT_PTR_NOT_NULL(l);
            struct elem *e = fetch_elem(l, 1);
            CU_ASSERT_PTR_NULL(e);
        }
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:matrix_set_test0:1 => [Matrice tableau : modification d'un élément]
void matrix_set_test0() {
    struct matrix *m;
    m = get_test_matrix1();
    assert(m);

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        int ret = matrix_set(m, 1, 3, 42.0);
        CU_ASSERT_EQUAL(ret, 0);
        CU_ASSERT_DOUBLE_EQUAL(m->elems[1][3], 42.0, 0.0001);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}