/*
 * TODO : Check if it works well
 */

// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_strcpy_return() {
    set_test_metadata("strcpy_impl", _("Check if the string is correctly put in memory"), 1);

    char *ret = NULL;

    const char *stack_src = "Chaine de char de test un peu courte mais pas trop quand meme";
    char *src = (char *)malloc(strlen(stack_src)+1);
    if (src == NULL)
        CU_FAIL("no mem");
    strcpy(src, stack_src);

    monitored.malloc = true;
    monitored.calloc = true;

    SANDBOX_BEGIN;
    ret = buf_strcpy(src);
    SANDBOX_END;


    // check if only one call to malloc
    int ms = stats.malloc.called;
    int cs = stats.calloc.called;
    CU_ASSERT_EQUAL(ms, 1);
    if (ms > 1){
        push_info_msg(_("You used more than one call to malloc"));
        return;
    }

    // check if new element is malloced
    int mal = malloced((void*) ret);
    CU_ASSERT_TRUE(mal);
    // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
    if (mal && ms) {
        if(stats.malloc.last_params.size != strlen(src)+1) {
            CU_FAIL("wrong malloc size");
            push_info_msg(_("The allocated memory doesn't the correct size."));
            set_tag("malloc_fail");
            return;
        }
        if (strncmp(ret, src, strlen(src) + 1) != 0){
            CU_FAIL("wrong string");
        }
        free(ret);
    }
    if(!mal){
        push_info_msg(_("The returned pointer is not malloced"));
        set_tag("malloc_fail");
    }
    if(cs){
        CU_FAIL();
        set_tag("malloc_fail");
        push_info_msg(_("You should use malloc for this task. Calloc could also work but it's not efficient to use it here since we initialise the memory just after we allocate it"));
    }
}

void test_strcpy_nomem() {
    set_test_metadata("strcpy_impl", _("Check the behavior of the function when the call to malloc fails"), 1);

    char *ret = NULL;
    char *src = "Chaine de char de test un peu courte mais pas trop quand meme";

    monitored.malloc = true;
    failures.malloc = FAIL_ALWAYS;
    failures.malloc_ret = NULL;

    SANDBOX_BEGIN;
    ret = buf_strcpy(src);
    SANDBOX_END;

    CU_ASSERT_PTR_NULL(ret);
    if (ret){
        push_info_msg(_("The return value of your implementation is wrong"));
        set_tag("malloc_fail");
    }

    free(ret);

}

int main(int argc,char** argv)
{
    BAN_FUNCS(memcpy, memccpy);
    RUN(test_strcpy_return, test_strcpy_nomem);
}
