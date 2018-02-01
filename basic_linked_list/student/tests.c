// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

#include<stdio.h>

/**
* Check if the @list correspond to the expected list represented by @tab
*/
int check_list(list_t* list, int *tab, int n){
  if (!list | !tab | (n < 0))
    return 2;

  int i;
  node_t *run = list->first;
  for(i =  0; i < n; i++){
    if (run->value != *(tab+i) || !run)
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

list_t* init_list_corr(){
  list_t *list = malloc(sizeof(list_t));
  if (!list)
    return NULL;
  list->first = NULL;
  list->size = 0;
  return list;
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
  if (!stats.malloc.called)
    push_info_msg(_("You used more than one call to malloc"));

  CU_ASSERT_TRUE(malloced((void*) ret));

  CU_ASSERT_PTR_NOT_NULL(ret);

  free_node_corr(ret);

}

void test_init_node_value(){
  set_test_metadata("init_node", _("Check the initialisation of the node"), 2);

  node_t* ret = NULL;

  SANDBOX_BEGIN;
  ret = init_node(10);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret->value, 10);
  if (ret->value != 10)
    push_info_msg(_("The value of the node is not the expected one"));

  CU_ASSERT_PTR_NULL(ret->next);
  if (ret->next)
    push_info_msg(_("The 'next' pointer is not correctly initialised"));

  free_node_corr(ret);

  int val = 25136984;

  SANDBOX_BEGIN;
  ret = init_node(val);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret->value, val);
  if (ret->value != val)
    push_info_msg(_("The value of the node is not the expected one"));

  CU_ASSERT_PTR_NULL(ret->next);
  if (ret->next)
    push_info_msg(_("The 'next' pointer is not correctly initialised"));

  free_node_corr(ret);
}

void test_init_node_nomem(){
  set_test_metadata("init_node", _("Check the return value if malloc call fails"), 1);

  node_t* ret = (node_t *) 0x42;

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;

  srandom(881468468);

  SANDBOX_BEGIN;
  ret = init_node(42);
  SANDBOX_END;
  printf("%p\n", ret);

  CU_ASSERT_PTR_NULL(ret);
  if (ret)
    push_info_msg(_("Wrong return value when malloc call fails"));
}

void test_add_node_empty(){
  set_test_metadata("add_node", _("Check the behavior of the function when the list is empty"), 1);

  list_t *list = init_list_corr();
  if (!list)
    return;

  int ret = 0;

  monitored.malloc = true;
  int value = 16328468;

  SANDBOX_BEGIN;
  ret = add_node(list, value);
  SANDBOX_END;

  // Tests
  //-----------------------------------------------------------------

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms != 1)
    push_info_msg(_("You used more than one call to malloc"));

  // check if new element is malloced
  int mal = malloced((void*) list->first);
  CU_ASSERT_TRUE(mal);
  // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
  if (mal){
    int val = list->first->value;
    CU_ASSERT_EQUAL(val, value);
    if (val != value)
      push_info_msg(_("The inserted value does not correspond to the waited one"));
  }
  else
    push_info_msg(_("The new node is not allocated"));

  // check the return code in normal case (expected 0)
  CU_ASSERT_EQUAL(ret, 0);
  if (ret != 0)
    push_info_msg(_("Your function returns an error in normal case!"));

  // check the update of the list size
  CU_ASSERT_EQUAL(list->size, 1);
  if (list->size != 1)
    push_info_msg(_("Wrong update of the list size."));

  //-----------------------------------------------------------------

  free_list_corr(list);
}

void test_add_node_wrong_args(){
      set_test_metadata("add_node", _("Check the return code of the function if wrong args"), 1);

    int ret;
    srandom(47826786);

    SANDBOX_BEGIN;
    ret = add_node(NULL, random());
    SANDBOX_END;

    CU_ASSERT_TRUE(ret);
    if (!ret)
         push_info_msg(_("Wrong return value when wrong args"));
}

void test_add_node_non_empty(){
  set_test_metadata("add_node", _("Check the behavior of the function when the list is not empty"), 1);

  list_t *list = init_list_corr();
  if (!list)
    return;

  int val[4] = {16328468, 135166, 97643564, 664872674};

  node_t *node1 = init_node_corr(*(val+1)), *node2 = init_node_corr(*(val+2)), *node3 = init_node_corr(*(val+3));
  if (!node1 | !node2 | !node3){
    CU_FAIL("NO MEMORY AVAILABLE");
    return;
  }

  node1->next = node2;
  node2->next = node3;
  list->first = node1;
  list->size = 3;

  int ret = 0;

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = add_node(list, *val);
  SANDBOX_END;

  // Tests
  //-----------------------------------------------------------------

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms != 1)
    push_info_msg(_("You used more than one call to malloc"));

  // check if new element is malloced
  int mal = malloced((void*) list->first);
  CU_ASSERT_TRUE(mal);
  // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
  if (mal){
    int valf = list->first->value;
    CU_ASSERT_EQUAL(valf, *val);
    if (valf != *val)
      push_info_msg(_("The inserted value does not correspond to the waited one"));
  }
  else
    push_info_msg(_("The new node is not allocated"));

  // check if the structure if the list changed
  int cl = check_list(list, val, 4);
  CU_ASSERT_EQUAL(cl, 0);
  if (cl == 1)
    push_info_msg(_("The new linked list does not correspond to the waited one"));
  else if (cl == 2)
    push_info_msg(_("SCRIPT ERROR"));

  // check the return code in normal case (expected 0)
  CU_ASSERT_EQUAL(ret, 0);
  if (ret != 0)
    push_info_msg(_("Your function returns an error in normal case!"));

  // check the update of the list size
  CU_ASSERT_EQUAL(list->size, 4);
  if (list->size != 4)
    push_info_msg(_("Wrong update of the list size."));

  //-----------------------------------------------------------------


  free_list_corr(list);
}

void test_add_node_nomem(){
  set_test_metadata("add_node", _("Check the behavior of the function when the malloc call fails"), 1);

  list_t *list = init_list_corr();
  if (!list)
    return;

  int val[4] = {16328468, 135166, 97643564, 664872674};

  node_t *node1 = init_node_corr(*(val+1)), *node2 = init_node_corr(*(val+2)), *node3 = init_node_corr(*(val+3));
  if (!node1 | !node2 | !node3){
    CU_FAIL("NO MEMORY AVAILABLE");
    return;
  }

  node1->next = node2;
  node2->next = node3;
  list->first = node1;
  list->size = 3;

  int ret = 0;

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;



  SANDBOX_BEGIN;
  ret = add_node(list, *val);
  SANDBOX_END;

  // Tests
  //-----------------------------------------------------------------

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms != 1)
    push_info_msg(_("You used more than one call to malloc"));

  // check if the structure if the list changed
  int *new_vals = (val+1);
  int cl = check_list(list, new_vals, 3);
  CU_ASSERT_EQUAL(cl, 0);
  if (cl == 1)
    push_info_msg(_("The new linked list does not correspond to the waited one"));
  else if (cl == 2)
    push_info_msg(_("SCRIPT ERROR"));

  // check the return code in normal case (expected 0)
  CU_ASSERT_EQUAL(ret, 1);
  if (ret != 1)
    push_info_msg(_("Your function returns an error in normal case!"));

  // check the update of the list size
  CU_ASSERT_EQUAL(list->size, 3);
  if (list->size != 3)
    push_info_msg(_("Wrong update of the list size."));

  //-----------------------------------------------------------------

  free_list_corr(list);
}

int main(int argc,char** argv)
{
    BAN_FUNCS(calloc);
    //RUN(test_init_node_alloc);
    RUN(test_init_node_alloc, test_init_node_value, test_init_node_nomem, test_add_node_empty, test_add_node_non_empty, test_add_node_nomem, test_add_node_wrong_args);
    //RUN(test_init_node_alloc, test_init_node_nomem, test_init_node_alloc);
}
