#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"


/*
* Check the result of an operation on a queue
* @q: queue to check
* @expected_size: expected size of the queue
* @values: values contained in the queue
* @method: 1 for successful enqueue, 0 otherwise
*/
int check_queue(queue_t* q, int expected_size, int* values, int method){
    if(q->size != expected_size) return -4; //size not modified
    node_t* runner = q->tail;
    int count = 0;
    while(runner->next != q->tail){
        if(count >= expected_size) return -2;
        if(method && runner == q->tail->next){
            if(!malloced(runner)) return -3;
        }
        if(runner->value != values[count]) return -1; //not the expected value
        if(count >= expected_size) return -2; //too many elements in the list
        count++;
        runner = runner->next;
    }
    if(count < expected_size - 1) return -2; //too few elements in the list
    return 0;
}

void error_msg(int ret, int fun){
  switch (ret) {
      case -4:
          push_info_msg(_("You have to update the size of the queue"));
          break;
      case -3:
          if (fun)
            push_info_msg(_("You have to malloc the enqueued element"));
          else
            push_info_msg(_("You have to free the dequeued element"));
          break;
      case -2:
          push_info_msg(_("You don't have the good number of elements in the queue"));
          break;
      case -1:
          push_info_msg(_("The elements of the queue are not in the good order"));
          break;
      default:
          break;
  }
}

void enqueue_mem_check(int delta, int stats, int ret){
  CU_ASSERT_EQUAL(delta,sizeof(node_t));
  if(delta != sizeof(node_t)) push_info_msg(_("You don't allocate the good amount of memory"));

  CU_ASSERT_EQUAL(stats,1);
  if(stats > 1) push_info_msg(_("Why do you use malloc more then once"));

  CU_ASSERT_EQUAL(ret,0);
  if(ret) push_info_msg(_("You don't return the good value"));
}

void test_enqueue_empty() {
    set_test_metadata("enqueue", _("Enqueue of an empty queue"), 1);

    queue_t q;
    q.tail = NULL;
    q.size = 0;

    int ret = -1;

    monitored.malloc = true;

    size_t start = stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;

    enqueue_mem_check(stats.memory.used - start, stats.malloc.called, ret);

    int values[] = {0};

    ret = check_queue(&q,1,values,1);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret,1);

    if(q.tail){
        CU_ASSERT_EQUAL(q.tail->next,q.tail);
        if(q.tail->next != q.tail)
            push_info_msg(_("You must link the tail with itself! That's a circular linked list"));
    }

    free(stats.malloc.last_return);
}

void test_enqueue_empty_fails() {
    set_test_metadata("enqueue", _("Enqueue of an empty queue when malloc fails"), 1);

    queue_t q;
    q.tail = NULL;

    int ret = 1;

    monitored.malloc = true;
    failures.malloc = 1;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;

    size_t delta = stats.memory.used -start;

    CU_ASSERT_EQUAL(delta,0);
    if(delta) push_info_msg(_("You shouldn't allocate memory when malloc fails"));

    CU_ASSERT_EQUAL(stats.malloc.called,1);
    if(stats.malloc.called > 1) push_info_msg(_("Why do you use malloc more then once"));

    CU_ASSERT_EQUAL(ret,-1);
    if(ret + 1) push_info_msg(_("You don't return the good value"));
}

void test_enqueue_one() {
    set_test_metadata("enqueue", _("Enqueue in a queue of size 1"), 1);

    node_t n = {NULL,1};
    n.next = &n;
    queue_t q;
    q.tail = &n;
    q.size = 1;

    int ret = -1;

    monitored.malloc = true;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;

    enqueue_mem_check(stats.memory.used - start, stats.malloc.called, ret);

    int values[] = {1,0};

    ret = check_queue(&q,2,values,1);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret,1);

    free(stats.malloc.last_return);
}

void test_enqueue_five() {
    set_test_metadata("enqueue", _("Enqueue in a queue of size 5"), 1);

    node_t n4 = {NULL,6};
    node_t n3 = {&n4,2};
    node_t n2 = {&n3,5};
    node_t n1 = {&n2,3};
    node_t n0 = {&n1,1};
    n4.next = &n0;
    queue_t q;
    q.tail = &n0;
    q.size = 5;

    int ret = -1;

    monitored.malloc = true;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;

    enqueue_mem_check(stats.memory.used - start, stats.malloc.called, ret);

    int values[] = {1,0,3,5,2,6};

    ret = check_queue(&q,6,values,1);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret,1);

    free(stats.malloc.last_return);
}

void test_enqueue_ten() {
    set_test_metadata("enqueue", _("Enqueue in a queue of size 10"), 1);

    node_t n9 = {NULL,-10};
    node_t n8 = {&n9,9};
    node_t n7 = {&n8,-3};
    node_t n6 = {&n7,4};
    node_t n5 = {&n6,7};
    node_t n4 = {&n5,6};
    node_t n3 = {&n4,2};
    node_t n2 = {&n3,5};
    node_t n1 = {&n2,3};
    node_t n0 = {&n1,1};
    n9.next = &n0;
    queue_t q;
    q.tail = &n0;
    q.size = 10;

    int ret = -1;

    monitored.malloc = true;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;

    enqueue_mem_check(stats.memory.used - start, stats.malloc.called, ret);

    int values[] = {1,0,3,5,2,6,7,4,-3,9,-10};

    ret = check_queue(&q,11,values,1);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret,1);

    free(stats.malloc.last_return);
}

void test_enqueue_ten_fails() {
    set_test_metadata("enqueue", _("Enqueue in a queue of size 10 and malloc fails"), 1);

    node_t n9 = {NULL,-10};
    node_t n8 = {&n9,9};
    node_t n7 = {&n8,-3};
    node_t n6 = {&n7,4};
    node_t n5 = {&n6,7};
    node_t n4 = {&n5,6};
    node_t n3 = {&n4,2};
    node_t n2 = {&n3,5};
    node_t n1 = {&n2,3};
    node_t n0 = {&n1,1};
    n9.next = &n0;
    queue_t q;
    q.tail = &n0;
    q.size = 10;

    int ret = -1;

    monitored.malloc = true;
    failures.malloc = 1;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = enqueue(&q,0);
    SANDBOX_END;

    size_t delta = stats.memory.used -start;

    CU_ASSERT_EQUAL(delta,0);
    if(delta) push_info_msg(_("You shouldn't allocate memory when malloc fails"));

    CU_ASSERT_EQUAL(stats.malloc.called,1);
    if(stats.malloc.called > 1) push_info_msg(_("Why do you use malloc more then once"));

    CU_ASSERT_EQUAL(ret,-1);
    if(ret + 1) push_info_msg(_("You don't return the good value"));

    int values[] = {1,3,5,2,6,7,4,-3,9,-10};

    ret = check_queue(&q,10,values,0);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret,1);

    free(stats.malloc.last_return);
}

void test_dequeue_one() {
    set_test_metadata("dequeue", _("Dequeue in a queue of size 1"), 1);

    node_t n = {NULL,1};
    n.next = &n;
    queue_t q;
    q.tail = &n;
    q.size = 1;

    int ret = -1;

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = dequeue(&q);
    SANDBOX_END;

    size_t delta = stats.memory.used -start;

    CU_ASSERT_EQUAL(delta,0);
    if(delta) push_info_msg(_("You shouldn't allocate memory for dequeue"));

    CU_ASSERT_EQUAL(stats.malloc.called,0);
    if(stats.malloc.called > 0) push_info_msg(_("Why do you use malloc?"));

    CU_ASSERT_EQUAL(stats.free.called,1);
    if(stats.free.called != 1) push_info_msg(_("You should use free once !"));

    CU_ASSERT_EQUAL(ret,1);
    if(ret != 1) push_info_msg(_("You don't return the good value"));

    CU_ASSERT_PTR_NULL(q.tail);
    if(q.tail) push_info_msg(_("You have to change the value of the tail to NULL for a queue of size 1"));

    CU_ASSERT_EQUAL(q.size,0);
    if(q.size) push_info_msg(_("You have to change the size of the queue after the dequeue"));
}

void test_dequeue_five() {
    set_test_metadata("dequeue", _("Dequeue in a queue of size 5"), 1);

    node_t n4 = {NULL,6};
    node_t n3 = {&n4,2};
    node_t n2 = {&n3,5};
    node_t n1 = {&n2,3};
    node_t n0 = {&n1,8};
    n4.next = &n0;
    queue_t q;
    q.tail = &n0;
    q.size = 5;

    int ret = -1;

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = dequeue(&q);
    SANDBOX_END;

    size_t delta = stats.memory.used -start;

    CU_ASSERT_EQUAL(delta,0);
    if(delta) push_info_msg(_("You shouldn't allocate memory for dequeue"));

    CU_ASSERT_EQUAL(stats.malloc.called,0);
    if(stats.malloc.called > 0) push_info_msg(_("Why do you use malloc?"));

    CU_ASSERT_EQUAL(stats.free.called,1);
    if(stats.free.called != 1) push_info_msg(_("You should use free once !"));

    CU_ASSERT_EQUAL(ret,8);
    if(ret != 8) push_info_msg(_("You don't return the good value"));

    int values[] = {6,3,5,2};

    ret = check_queue(&q,4,values,0);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret,0);

    free(stats.malloc.last_return);
}

void test_dequeue_ten() {
    set_test_metadata("dequeue", _("Dequeue in a queue of size 10"), 1);

    node_t n9 = {NULL,-10};
    node_t n8 = {&n9,9};
    node_t n7 = {&n8,-3};
    node_t n6 = {&n7,4};
    node_t n5 = {&n6,7};
    node_t n4 = {&n5,6};
    node_t n3 = {&n4,2};
    node_t n2 = {&n3,5};
    node_t n1 = {&n2,3};
    node_t n0 = {&n1,1};
    n9.next = &n0;
    queue_t q;
    q.tail = &n0;
    q.size = 10;

    int ret = -1;

    monitored.malloc = true;
    monitored.free = true;
    failures.free = FAIL_ALWAYS;

    size_t start= stats.memory.used;

    SANDBOX_BEGIN;
    ret = dequeue(&q);
    SANDBOX_END;

    size_t delta = stats.memory.used -start;

    CU_ASSERT_EQUAL(delta,0);
    if(delta) push_info_msg(_("You shouldn't allocate memory for dequeue"));

    CU_ASSERT_EQUAL(stats.malloc.called,0);
    if(stats.malloc.called > 0) push_info_msg(_("Why do you use malloc?"));

    CU_ASSERT_EQUAL(stats.free.called,1);
    if(stats.free.called != 1) push_info_msg(_("You should use free once !"));

    CU_ASSERT_EQUAL(ret,1);
    if(ret != 1) push_info_msg(_("You don't return the good value"));

    int values[] = {-10,3,5,2,6,7,4,-3,9};

    ret = check_queue(&q,9,values,0);

    CU_ASSERT_EQUAL(ret,0);

    error_msg(ret, 0);

    free(stats.malloc.last_return);
}


int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_enqueue_empty,test_enqueue_empty_fails,test_enqueue_one,test_enqueue_five,test_enqueue_ten,test_enqueue_ten_fails,test_dequeue_one,test_dequeue_five,test_dequeue_ten);
}
