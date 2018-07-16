#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_get_test0:1 => [Matrice creuse: récupération du premier élément]
void sp_matrix_get_test0() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
	assert(m);
	assert(sp_check_coherency(m));
    
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        double val = sp_matrix_get(m, 2, 0);
        CU_ASSERT_DOUBLE_EQUAL(val, 4.0, m->precision);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_get_test1:1 => [Matrice creuse: récupération du dernier élément]
void sp_matrix_get_test1() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
	assert(m);
	assert(sp_check_coherency(m));
    
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        double val = sp_matrix_get(m, 1, 2);
        CU_ASSERT_DOUBLE_EQUAL(val, 2.0, m->precision);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_get_test2:1 => [Matrice creuse: récupération d'un élément central]
void sp_matrix_get_test2() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
	assert(m);
	assert(sp_check_coherency(m));
    
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        double val = sp_matrix_get(m, 1, 2);
        CU_ASSERT_DOUBLE_EQUAL(val, 2.0, m->precision);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_get_test3:1 => [Matrice creuse: récupération d'un zéro]
void sp_matrix_get_test3() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
	assert(m);
	assert(sp_check_coherency(m));
    
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        double val = sp_matrix_get(m, 1, 0);
        CU_ASSERT_DOUBLE_EQUAL(val, 0.0, m->precision);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:sp_matrix_get_test4:1 => [Matrice creuse: récupération sur une ligne de zéros]
void sp_matrix_get_test4() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
	assert(m);
	assert(sp_check_coherency(m));
    
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        double val = sp_matrix_get(m, 0, 0);
        CU_ASSERT_DOUBLE_EQUAL(val, 0.0, m->precision);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}

// @p1archive:matrix_get_test0:1 => [Matrice tableau: récupération d'un élément]
void matrix_get_test0() {
    struct matrix *m;
    m = get_test_matrix1();
	assert(m);
    
    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        double val = matrix_get(m, 4, 1);
        CU_ASSERT_DOUBLE_EQUAL(val, 8.0, 0.0001);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
}
