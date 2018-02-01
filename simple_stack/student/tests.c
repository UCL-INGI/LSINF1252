// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

struct node *init_node(int value){
  struct node *node = (struct node*) malloc(sizeof(struct node));
  if (!node)
    return NULL;

  node->next = NULL;
  node->value = value;

  return node;
}

void free_stack(struct node *head){
  if (!head)
    return;

  struct node *run = head, *tmp;
  while (!run){
    tmp = run;
    run = run->next;
    free(tmp->next);
    free(tmp);
  }

  free(head);
}

struct node *generate_stack(int *array){
  int i, seed = 3167359;
  srandom(seed);
  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return NULL;

  for (i = 0; i < 5; i++){
    //printf("seed %i = %i\n", i, seed);
    struct node *node = (struct node*) malloc(sizeof(struct node));
    if (!node){
      free_stack(head);
      return NULL;
    }
    *(array+i) = seed;
    node->next = head;
    node->value = seed;
    seed = random();
    head = node;
  }

  return head;
}

void test_push_param_nomem() {
  set_test_metadata("push", _("Check if the parameter and the allocation of the new node"), 1);

  int ret;

  srandom(614646841);

  SANDBOX_BEGIN;
  ret = push(NULL, random());
  SANDBOX_END;

  CU_ASSERT_EQUAL(1, ret);
  if (ret)
      push_info_msg(_("Wrong return code if wrong args"));

  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return;
  head->value = 10;
  head->next = NULL;

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;

  SANDBOX_BEGIN;
  ret = push(&head, 164684);
  SANDBOX_END;
    
  CU_ASSERT_EQUAL(1, ret);
  if (ret)
     push_info_msg(_("Wrong return code if wrong args"));
  
  free(head);
}

void test_push_general() {
  set_test_metadata("push", _("Check if the insertion of the new node is correct"), 1);

  int *array = (int*) malloc(sizeof(int)*6);
  struct node *stack = generate_stack(array);
  if (!stack)
    return;

  int ret, value = random();

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = push(&stack, value);
  SANDBOX_END;

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms > 1)
    push_info_msg(_("You used more than one call to malloc"));

  printf("%i", ret);

  // check if new element is malloced
  int mal = malloced((void*) stack);
  CU_ASSERT_TRUE(mal);
  // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
  if (mal){
    CU_ASSERT_EQUAL(value, stack->value);
    if (value != stack->value)
      push_info_msg(_("The pushed value differ from the expected one"));
  }
  else
    push_info_msg(_("The returned pointer is not malloced"));

  if (!stack && !stack->next){
    CU_FAIL("The node after the head node is not correctly initialised");
    return;
  }


  *(array+5) = value;

  int i;

  struct node *run = stack;
  for (i = 0; i < 2; i++){
    //printf("array: %i => run->value: %i\n", *(array+5-i), run->value);
    if (!run || *(array+5-i) != run->value){
      CU_FAIL("The structure of the stack has changed");
      push_info_msg(_("The structure of the stack has changed"));
      return;
    }
    run = run->next;
  }

  CU_PASS("");

  free_stack(stack);

}

void test_push_empty(){

}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_push_param_nomem, test_push_general);
}
