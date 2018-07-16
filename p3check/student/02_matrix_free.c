#include "tests.h"
#include "matrix.h"

// @p1archive:sp_matrix_free_test0:1 => [Destruction d'une matrice creuse]
void sp_matrix_free_test0() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));
    
    bzero(&stats,sizeof(stats));
    monitored.free=true;

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        sp_matrix_free(m);
        CU_ASSERT_TRUE(stats.free.called>=1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
    
    monitored.free=false;
}

// @p1archive:matrix_free_test0:1 => [Destruction d'une matrice tableau]
void matrix_free_test0() {
    struct matrix *m = get_test_matrix1();
    assert(m);
    
    bzero(&stats,sizeof(stats));
    monitored.free=true;

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        matrix_free(m);
        CU_ASSERT_TRUE(stats.free.called>=1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
    
    monitored.free=false;
}

// @p1archive:sp_matrix_free_test1:1 => [Destruction d'une matrice creuse avec malloc qui échoue]
void sp_matrix_free_test1() {
    struct sp_matrix *m;
    m = sp_get_test_matrix1();
    assert(m);
    assert(sp_check_coherency(m));
    
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    monitored.free=true;

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        sp_matrix_free(m);
        CU_ASSERT_TRUE(stats.free.called>=1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
    
    monitored.malloc=false;
    monitored.calloc=false;
    monitored.free=false;
}

// @p1archive:matrix_free_test1:1 => [Destruction d'une matrice tableau avec malloc qui échoue]
void matrix_free_test1() {
    struct matrix *m = get_test_matrix1();
    assert(m);
    
    bzero(&stats,sizeof(stats));
    monitored.malloc=true;
    monitored.calloc=true;
    failures.malloc=FAIL_ONCE;
    failures.calloc=FAIL_ONCE;
    monitored.free=true;

    start_timer();
    if(sigsetjmp(segv,1) == 0) {
        matrix_free(m);
        CU_ASSERT_TRUE(stats.free.called>=1);
    } else {
        CU_FAIL("Segmentation Fault");
    }
    stop_timer();
    
    monitored.malloc=false;
    monitored.calloc=false;
    monitored.free=false;
}