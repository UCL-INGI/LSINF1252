#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_success() {
    set_test_metadata("sleep_malloc", _("Malloc always success"), 1);

    void* ret = NULL;

    monitored.malloc = true;
    failures.malloc = FAIL_NEVER;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 16);
    if (used_size != 16)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_PTR_NOT_NULL(ret);

    CU_ASSERT_TRUE(malloced(ret));

    CU_ASSERT_EQUAL(stats.malloc.called,1)
    if(stats.malloc.called > 1) push_info_msg(_("Why did you call malloc more then once?"));
    

    free(ret);
}

void test_fail_first() {
    set_test_metadata("sleep_malloc", _("Malloc fails first"), 1);

    void* ret = NULL;

    monitored.malloc = true;
    failures.malloc = FAIL_FIRST;
    monitored.sleep = true;
    failures.sleep = FAIL_ALWAYS;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 16);
    if (used_size != 16)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_EQUAL(stats.sleep.last_arg,5);
    if(stats.sleep.last_arg != 5){
        push_info_msg(_("You did not call sleep with the right time. See `sleep(3) <https://sites.uclouvain.be/SystInfo/manpages/man3/sleep.3.html>`_ for more information"));
    }

    CU_ASSERT_PTR_NOT_NULL(ret);

    CU_ASSERT_TRUE(malloced(ret));

    CU_ASSERT_EQUAL(stats.malloc.called,2)
    if(stats.malloc.called > 2) push_info_msg(_("Why did you call malloc more then once per loop?"));

    CU_ASSERT_EQUAL(stats.sleep.called,1)
    if(stats.sleep.called > 1) push_info_msg(_("Why did you call sleep more then once per loop?"));

    free(ret);
}

void test_fail_twice() {
    set_test_metadata("sleep_malloc", _("Malloc fails twice"), 1);

    void* ret = NULL;

    monitored.malloc = true;
    failures.malloc = FAIL_TWICE;
    monitored.sleep = true;
    failures.sleep = FAIL_ALWAYS;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 16);
    if (used_size != 16)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_PTR_NOT_NULL(ret);

    CU_ASSERT_TRUE(malloced(ret));

    CU_ASSERT_EQUAL(stats.sleep.last_arg,5);
    if(stats.sleep.last_arg != 5){
        push_info_msg(_("You did not call sleep with the right time. See `sleep(3) <https://sites.uclouvain.be/SystInfo/manpages/man3/sleep.3.html>`_ for more information"));
    }

    CU_ASSERT_EQUAL(stats.malloc.called,3)
    if(stats.malloc.called > 3) push_info_msg(_("Why did you call malloc more then once per loop?"));

    CU_ASSERT_EQUAL(stats.sleep.called,2)
    if(stats.sleep.called > 2) push_info_msg(_("Why did you call sleep more then once per loop?"));

    free(ret);
}

void test_fail_five() {
    set_test_metadata("sleep_malloc", _("Malloc fails five first times"), 1);

    void* ret = NULL;

    monitored.malloc = true;
    failures.malloc = 0x1f;
    monitored.sleep = true;
    failures.sleep = FAIL_ALWAYS;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 16);
    if (used_size != 16)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_PTR_NOT_NULL(ret);

    CU_ASSERT_TRUE(malloced(ret));

    CU_ASSERT_EQUAL(stats.sleep.last_arg,5);
    if(stats.sleep.last_arg != 5){
        push_info_msg(_("You did not call sleep with the right time. See `sleep(3) <https://sites.uclouvain.be/SystInfo/manpages/man3/sleep.3.html>`_ for more information"));
    }

    CU_ASSERT_EQUAL(stats.malloc.called,6)
    if(stats.malloc.called > 6) push_info_msg(_("Why did you call malloc more then once per loop?"));

    CU_ASSERT_EQUAL(stats.sleep.called,5)
    if(stats.sleep.called > 5) push_info_msg(_("Why did you call sleep more then once per loop?"));

    free(ret);
}

void test_fail_eight() {
    set_test_metadata("sleep_malloc", _("Malloc fails eight first times"), 1);

    void* ret = NULL;

    monitored.malloc = true;
    failures.malloc = 0xff;
    monitored.sleep = true;
    failures.sleep = FAIL_ALWAYS;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 16);
    if (used_size != 16)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_PTR_NOT_NULL(ret);

    CU_ASSERT_TRUE(malloced(ret));

    CU_ASSERT_EQUAL(stats.sleep.last_arg,5);
    if(stats.sleep.last_arg != 5){
        push_info_msg(_("You did not call sleep with the right time. See `sleep(3) <https://sites.uclouvain.be/SystInfo/manpages/man3/sleep.3.html>`_ for more information"));
    }

    CU_ASSERT_EQUAL(stats.malloc.called,9)
    if(stats.malloc.called > 9) push_info_msg(_("Why did you call malloc more then once per loop?"));

    CU_ASSERT_EQUAL(stats.sleep.called,8)
    if(stats.sleep.called > 8) push_info_msg(_("Why did you call sleep more then once per loop?"));

    free(ret);
}

void test_fail_ten() {
    set_test_metadata("sleep_malloc", _("Malloc fails ten first times"), 1);

    void* ret = (void*) 0x86;

    monitored.malloc = true;
    failures.malloc = 0x3ff;
    monitored.sleep = true;
    failures.sleep = FAIL_ALWAYS;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 0);
    if (used_size != 0)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_PTR_NULL(ret);

    CU_ASSERT_EQUAL(stats.sleep.last_arg,5);
    if(stats.sleep.last_arg != 5){
        push_info_msg(_("You did not call sleep with the right time. See `sleep(3) <https://sites.uclouvain.be/SystInfo/manpages/man3/sleep.3.html>`_ for more information"));
    }

    CU_ASSERT_EQUAL(stats.malloc.called,10)
   // if(stats.malloc.called > 10) push_info_msg(_("Why did you call malloc more then once per loop?"));

    CU_ASSERT_EQUAL(stats.sleep.called,10)
    if(stats.sleep.called > 10) push_info_msg(_("Why did you call sleep more then once per loop?"));
}

void test_fail_always() {
    set_test_metadata("sleep_malloc", _("Malloc fails always"), 1);

    void* ret = (void*) 0x86;

    monitored.malloc = true;
    failures.malloc = FAIL_ALWAYS;
    monitored.sleep = true;
    failures.sleep = FAIL_ALWAYS;

    size_t  start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = sleep_malloc(16);
    SANDBOX_END;

    size_t used_size = stats.memory.used - start;

    CU_ASSERT_EQUAL(used_size, 0);
    if (used_size != 0)
      push_info_msg(_("You allocated more memory than required."));

    CU_ASSERT_PTR_NULL(ret);

    CU_ASSERT_EQUAL(stats.sleep.last_arg,5);
    if(stats.sleep.last_arg != 5){
        push_info_msg(_("You did not call sleep with the right time. See `sleep(3) <https://sites.uclouvain.be/SystInfo/manpages/man3/sleep.3.html>`_ for more information"));
    }

    CU_ASSERT_EQUAL(stats.malloc.called,10)
    if(stats.malloc.called > 10) push_info_msg(_("Why did you call malloc more then once per loop?"));

    CU_ASSERT_EQUAL(stats.sleep.called,10)
    if(stats.sleep.called > 10) push_info_msg(_("Why did you call sleep more then once per loop?"));
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_success,test_fail_first,test_fail_twice,test_fail_five,test_fail_eight,test_fail_ten,test_fail_always);
}
