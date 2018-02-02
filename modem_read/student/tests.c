#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

int8_t* tab1;

void test_42_right1() {
    set_test_metadata("modem", _("test1 for 42 contained"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 0;

    int i;
    for(i = 0; i < 64; i++){
        tab1[i] = i;
    }

    void *p = trap_buffer(64, TRAP_RIGHT, PROT_READ, (void*) tab1);

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;
    failures.malloc = 1;
    failures.malloc_ret = p;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(stats.malloc.called,1);
    CU_ASSERT_EQUAL(stats.free.called,1);

    free_trap(p,64);
}

void test_42_left1() {
    set_test_metadata("modem", _("test2 for 42 contained, left trapped"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 0;

    int i;
    for(i = 0; i < 64; i++){
        tab1[i] = i;
    }

    void * p = trap_buffer(64, TRAP_LEFT, PROT_READ, (void*) tab1);

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;
    failures.malloc = 1;
    failures.malloc_ret = p;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(stats.malloc.called,1);
    CU_ASSERT_EQUAL(stats.free.called,1);
    
    if (stats.malloc.last_params.size != 256) { 
        CU_FAIL();
        push_info_msg(_("You didn't malloc() with the right size"));
    }

    free_trap(p,64);
}

void test_42_right2() {
    set_test_metadata("modem", _("test2 for 42 contained"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 0;

    int i;
    for(i = 0; i < 64; i++){
        tab1[i] = 64 - i;
    }

    void * p = trap_buffer(64, TRAP_RIGHT, PROT_READ, (void*) tab1);

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;
    failures.malloc = 1;
    failures.malloc_ret = p;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(stats.malloc.called,1);
    CU_ASSERT_EQUAL(stats.free.called,1);

    free_trap(p,64);
}

void test_42_left2() {
    set_test_metadata("modem", _("test2 for 42 contained, left trapped"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 0;

    int i;
    for(i = 0; i < 64; i++){
        tab1[i] = 64 - i;
    }

    void * p = trap_buffer(64, TRAP_LEFT, PROT_READ, (void*) tab1);

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;
    failures.malloc = 1;
    failures.malloc_ret = p;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;


    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(stats.malloc.called,1);
    CU_ASSERT_EQUAL(stats.free.called,1);

    free_trap(p,64);
}

void test_no42_right() {
    set_test_metadata("modem", _("test2 for 42 not contained"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 1;

    int i;
    for(i = 0; i < 64; i++){
        tab1[i] = 128 - i;
    }

    void *p = trap_buffer(64, TRAP_RIGHT, PROT_READ, (void*) tab1);

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;
    failures.malloc = FAIL_ALWAYS;
    failures.malloc_ret = p;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);
    CU_ASSERT_EQUAL(stats.malloc.called,1);
    CU_ASSERT_EQUAL(stats.free.called,1);

    free_trap(p,64);
}

void test_no42_left() {
    set_test_metadata("modem", _("test2 for 42 not contained, left trapped"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 1;

    int i;
    for(i = 0; i < 64; i++){
        tab1[i] = 128 - i;
    }

    void * p = trap_buffer(64, TRAP_LEFT, PROT_READ, (void*) tab1);

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;
    failures.malloc = 1;
    failures.malloc_ret = p;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    CU_ASSERT_EQUAL(stats.malloc.called,1);

    CU_ASSERT_EQUAL(stats.free.called,1);

    free_trap(p,64);
}

void test_fail() {
    set_test_metadata("modem", _("test for malloc failed"), 1);

    if(!tab1) CU_FAIL(_("malloc failed"));

    int ret = 1;

    monitored.malloc = true;
    monitored.free = true;
    failures.malloc = FAIL_ALWAYS;
    SANDBOX_BEGIN;
    ret = has_42();
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,-1);

    CU_ASSERT_EQUAL(stats.malloc.called,1);

    CU_ASSERT_EQUAL(stats.free.called,0);
}

int main(int argc,char** argv)
{
    tab1 = (int8_t*) malloc(64);
    BAN_FUNCS();
    RUN(test_42_right1, test_42_right2, test_no42_right, test_fail, test_42_left2, test_no42_left, test_42_left1);
    free(tab1);
}
