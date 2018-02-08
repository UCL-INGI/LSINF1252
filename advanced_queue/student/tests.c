// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_enqueue_empty() {
    set_test_metadata("enqueue", _("Enqueue of an empty list"), 1);
    
    node_t* n = NULL;
    queue_t q;
    q.head = &n;

    int ret = -1;
    
    monitored.malloc = true;
    
    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;
    
    size_t delta = stats.memory.used -start;
    
    CU_ASSERT_EQUAL(delta,sizeof(node_t));
    
    CU_ASSERT_EQUAL(stats.malloc.called,1);

    CU_ASSERT_EQUAL(ret,0);
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_enqueue_empty);
}

