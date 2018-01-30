// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void free_node_corr(node_t* node){
  if (!node)
    return;

  if (!node->next)
    free(node->next);

  free(node);
}

void free_list_corr(list_t* list){
  if (!list)
    return;

  node_t* run = list->first, *tmp;
  while(run){
    tmp = run;
    run = run->next;
    free_node_corr(tmp);
  }

  free(list);
}

void test_init_node_alloc(){
  set_test_metadata("init_node", _("Check memory allocation of the node"), 1);

  node_t* ret = NULL;

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = init_node(0);
  SANDBOX_END;

  CU_ASSERT_EQUAL(stats.malloc.called, 1);
  CU_ASSERT_TRUE(malloced((void*) ret));
  CU_ASSERT_PTR_NOT_NULL(ret);

  free_node_corr(ret);

}

void test_init_node_value(){
  set_test_metadata("init_node", _("Check the value of the node"), 1);

  node_t* ret = NULL;

  SANDBOX_BEGIN;
  ret = init_node(10);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret->value, 10);
  CU_ASSERT_PTR_NULL(ret->next);
  free_node_corr(ret);

  SANDBOX_BEGIN;
  ret = init_node(25136984);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret->value, 25136984);
  CU_ASSERT_PTR_NULL(ret->next);

  free_node_corr(ret);
}

void test_init_node_nomem(){
  set_test_metadata("init_node", _("Check the return value if malloc call fails"), 1);

  node_t* ret = NULL;

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;

  SANDBOX_BEGIN;
  ret = init_node(0);
  SANDBOX_END;

  CU_ASSERT_PTR_NULL(ret);
}

void test_add_node_empty(){
  set_test_metadata("add_node", _("Check the behavior of the function when the list is empty"), 1);

  list_t *list = malloc(sizeof(list_t));
  if (!list)
    CU_FAIL("NO MEMORY AVAILABLE");
  list->first = NULL;
  list->size = 0;

  int ret = 0;

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = add_node(list, 16328468);
  SANDBOX_END;

  CU_ASSERT_EQUAL(stats.malloc.called, 1);
  CU_ASSERT_TRUE(malloced((void*) list->first));

  CU_ASSERT_EQUAL(ret, 0);
  if (ret != 0){
    push_info_msg(_("Your function returns an error in normal case!"));
  }

  free_list_corr(list);

}


int main(int argc,char** argv)
{
    RUN(test_init_node_alloc, test_init_node_value, test_init_node_nomem, test_add_node_empty) ;
}
