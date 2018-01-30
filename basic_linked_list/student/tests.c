// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

/**
* Check if the @list correspond to the expected list represented by @tab
*/
int check_list(list_t* list, int *tab, int n){
  if (!list | !tab | (n < 0))
    return 2;

  int i;
  node_t *run = list->first;
  for(i =  0; i < n; i++){
    if (!run)
      return 2;
    if (run->value != *(tab+i))
      return 1;
    run = run->next;
  }
  return 0;
}

node_t* init_node_corr(int value){
  node_t* ret = malloc(sizeof(node_t));
  if (!ret)
    return NULL;

  ret->next = NULL;
  ret->value = value;
  return ret;
}

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

  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms != 1)
    push_info_msg(_("You used more than one call to malloc"));
    
  CU_ASSERT_TRUE(malloced((void*) list->first));
  CU_ASSERT_EQUAL(list->first->value, 16328468);

  CU_ASSERT_EQUAL(ret, 0);
  if (ret != 0)
    push_info_msg(_("Your function returns an error in normal case!"));


  free_list_corr(list);

}

void test_add_node_non_empty(){
  set_test_metadata("add_node", _("Check the behavior of the function when the list is not empty"), 1);

  list_t *list = malloc(sizeof(list_t));
  if (!list)
    CU_FAIL("NO MEMORY AVAILABLE");
  list->first = NULL;
  list->size = 0;

  int val[4] = {16328468, 135166, 97643564, 664872674};

  node_t *node1 = init_node(*(val+1)), *node2 = init_node(*(val+2)), *node3 = init_node(*(val+3));
  if (!node1 | !node2 | !node3)
    CU_FAIL("NO MEMORY AVAILABLE");

  node1->next = node2;
  node2->next = node3;
  list->first = node1;
  list->size = 3;

  int ret = 0;

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = add_node(list, *val);
  SANDBOX_END;

  int cl = check_list(list, val, 4);
  CU_ASSERT_EQUAL(cl, 0);
  if (cl == 1)
    push_info_msg(_("The new linked list does not correspond to the waited one"));
  else if (cl == 2)
    push_info_msg(_("SCRIPT ERROR"));

  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms != 1)
    push_info_msg(_("You used more than one call to malloc"));

  int mal = malloced((void*) list->first);
  CU_ASSERT_TRUE(mal);
  if (mal != 1)
    push_info_msg(_("The new element does not use malloc"));

  int new_val = list->first->value;
  CU_ASSERT_EQUAL(new_val, *val);
  if (new_val != *val)
    push_info_msg(_("The inserted value does not correspond to the waited one"));

  CU_ASSERT_EQUAL(ret, 0);
  if (ret != 0)
    push_info_msg(_("Your function returns an error in normal case!"));


  free_list_corr(list);
}

int main(int argc,char** argv)
{
    RUN(test_init_node_alloc, test_init_node_value, test_init_node_nomem, test_add_node_empty, test_add_node_non_empty) ;
}
