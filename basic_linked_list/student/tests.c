// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void free_node_corr(node_t* node){
  if (!node->next)
    free(node->next);

  free(node);
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


int main(int argc,char** argv)
{
    RUN(test_init_node_alloc, test_init_node_value, test_init_node_nomem) ;
}
