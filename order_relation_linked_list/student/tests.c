#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"
#include "CTester/CTester.h"

int cmp1(char a, char b){
    return a - b;
}

int cmp2(char a, char b){
    return b - a;
}

int cmp3(char a, char b){
    int res = (a%2 > 0) - (b%2 > 0);
    if(!res) return a-b;
    return res;
}

void test_compare_equals() {
    set_test_metadata("order_relation", _("Comparison of equals char"), 1);

    int ret = 1;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = compare('a','a');
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    ret = 1;

    SANDBOX_BEGIN;
    ret = compare('z','z');
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    ret = 1;

    SANDBOX_BEGIN;
    ret = compare('\0','\0');
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    CU_ASSERT_EQUAL(stats.malloc.called,0);

    CU_ASSERT_EQUAL(stats.free.called,0);
    if (stats.free.called || stats.malloc.called) {
        push_info_msg(_("Why do you use malloc or free in compare"));
    }
}

void test_compare_greater() {
    set_test_metadata("order_relation", _("Comparison with a > b"), 1);

    int ret = 0;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = compare('a','Z');
    SANDBOX_END;

    CU_ASSERT_TRUE(ret > 0);

    ret = 0;

    SANDBOX_BEGIN;
    ret = compare('z','y');
    SANDBOX_END;

    CU_ASSERT_TRUE(ret > 0);

    ret = 0;

    SANDBOX_BEGIN;
    ret = compare('\n','\0');
    SANDBOX_END;

    CU_ASSERT_TRUE(ret > 0);

    CU_ASSERT_EQUAL(stats.malloc.called,0);

    CU_ASSERT_EQUAL(stats.free.called,0);
    if (stats.free.called || stats.malloc.called) {
        push_info_msg(_("Why do you use malloc or free in compare"));
    }
}

void test_compare_smaller() {
    set_test_metadata("order_relation", _("Comparison with a < b"), 1);

    int ret = 0;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = compare('Z','a');
    SANDBOX_END;

    CU_ASSERT_TRUE(ret < 0);

    ret = 0;

    SANDBOX_BEGIN;
    ret = compare('y','z');
    SANDBOX_END;

    CU_ASSERT_TRUE(ret < 0);

    ret = 0;

    SANDBOX_BEGIN;
    ret = compare('\0','\n');
    SANDBOX_END;

    CU_ASSERT_TRUE(ret < 0);

    CU_ASSERT_EQUAL(stats.malloc.called,0);

    CU_ASSERT_EQUAL(stats.free.called,0);
    if (stats.free.called || stats.malloc.called) {
        push_info_msg(_("Why do you use malloc or free in compare"));
    }
}

void test_empty_list(){
    set_test_metadata("insert", _("Insertion in an empty list"), 1);

    int ret = 1;
    node_t* l = NULL;
    node_t** h = &l;

    monitored.malloc = 1;

    size_t start = stats.memory.used;
    SANDBOX_BEGIN;
    ret = insert(h,'c',&cmp1);
    SANDBOX_END;
    size_t delta = stats.memory.used - start;

    CU_ASSERT_EQUAL(delta, sizeof(node_t));

    CU_ASSERT_EQUAL(ret,0);

    CU_ASSERT_PTR_NOT_NULL(*h);

    CU_ASSERT_TRUE(malloced(*h));

    CU_ASSERT_EQUAL(stats.malloc.called,1);

    if(*h){
        CU_ASSERT_EQUAL((*h)->val,'c');
        CU_ASSERT_PTR_NULL((*h)->next);
    }
}

void test_empty_list_fail(){
    set_test_metadata("insert", _("Insertion in an empty list with malloc fail"), 1);

    int ret = 1;
    node_t* l = NULL;
    node_t** h = &l;

    monitored.malloc = 1;
    failures.malloc = 1;

    size_t start = stats.memory.used;
    SANDBOX_BEGIN;
    ret = insert(h,'c',&cmp1);
    SANDBOX_END;
    size_t delta = stats.memory.used - start;

    CU_ASSERT_EQUAL(delta, 0);

    CU_ASSERT_EQUAL(ret,-1);

    CU_ASSERT_PTR_NULL(*h);

    CU_ASSERT_EQUAL(stats.malloc.called,1);
}


int main(int argc,char** argv)
{
    BAN_FUNCS(free);
    RUN(test_compare_equals, test_compare_greater, test_compare_smaller,test_empty_list);
}
