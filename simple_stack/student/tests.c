// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

#include <string.h>

void print_array_as_stack(char **array, int l){
  int i;
  for (i=l-1; i>=0; i-- ){
    printf("array as stack %s\n", *(array+i));
  }
}

void print_stack(struct node *head, int l){
  if (!head){
    printf("NO stack");
    return;
  }

  int i;
  struct node *run = head;
  printf("head:  ");
  for (i =0; i<l; i++){
    printf("%s\n\t|\n\tv\n", run->name);
    run = run->next;
  }
  printf("\tNULL\n");
}

int check_stack(struct node *head, char** array, int len){
  if (!head){
    if (!array && len == 0)
      return 0;
    else
      return 1;
  }

  // check if the structure of the stack correspond to array
  int i, l = len-1;
  struct node *run = head;
  char *src, *st;

  for (i = 0; i < l; i++){
    src = *(array+l-i);
    st = run->name;

    //printf("array = %s, name = %s\n", src, st);
    if (!run || strcmp(src, st) != 0){
      return 1;
    }
    run = run->next;
  }

  return 0;
}

void free_stack(struct node *head){
  if (!head)
    return;

  struct node *run = head, *tmp;
  while (!run){
    tmp = run;
    run = run->next;
    free(tmp->name);
    free(tmp);
  }
  free(head);
}

struct node *generate_stack(char **array, int l){
  struct node *head = NULL;
  char * str;
  int i, len;

  for (i=0; i<l; i++){
    str = *(array+i);
    len = strlen(str) + 1;
    char *name = (char*) malloc(len);
    if (!name)
      return NULL;

    struct node *node = (struct node*) malloc(sizeof(struct node));
    if (!node)
      return NULL;

    strncpy(name, str, len);
    node->name = name;
    node->next = head;

    //printf("stack = %s\n", name);

    head = node;

  }

  return head;
}

/* Check the behavior of the function if the given @head is NULL or if the malloc call fails
*/
void test_push_param_nomem() {
  set_test_metadata("push", _("Check the behavior of the function if the given @head is NULL or if the malloc call fails"), 1);

  int ret;

  SANDBOX_BEGIN;
  ret = push(NULL, "test");
  SANDBOX_END;

  // check return value when null arg
  CU_ASSERT_EQUAL(1, ret);
  if (!ret)
      push_info_msg(_("Wrong return code if wrong args"));

  // check the return value if malloc call fails
  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return;
  head->name = "test";
  head->next = NULL;

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;

  SANDBOX_BEGIN;
  ret = push(&head, "ksdhfsfqsdfh");
  SANDBOX_END;

  CU_ASSERT_EQUAL(1, ret);
  if (!ret)
     push_info_msg(_("Wrong return code if wrong args"));

  free(head);
}

void test_push_changing_param(){
  set_test_metadata("push", _("Check if the function change @value"), 1);

  //int ret;

  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return;
  head->name = "test";
  head->next = NULL;

  char *src = "ksdhfsfqsdfh";
  int len = strlen(src);

  char *src_cpy = (char*) malloc(len);
  if (!src_cpy)
    return;

  strncpy(src_cpy, src, len+1);

  SANDBOX_BEGIN;
  push(&head, src);
  SANDBOX_END;
  printf("src = %s, src_cpy = %s\n", src, src_cpy);
  int cmp = strcmp((const char*) src, (const char*) src_cpy);
  CU_ASSERT_TRUE(!cmp);
  if (cmp != 0)
    push_info_msg(_("The function changed the value of @value"));

  free_stack(head);
}

void test_push_general() {
  set_test_metadata("push", _("Check if the insertion of the new node is correct"), 1);

  char *a[7] = {"fqgrsrfgfg", "moty;oe26rbgs",
  "i;rvqr6tgbsecr26", "5qvf15rg5g", "42",
   "r157g1srq7v16zs6", ""};

  struct node *stack = generate_stack(a, 6);
  if (!stack)
    return;

  int ret;

  char *src ="e5f1vq6f8e81E";

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = push(&stack, src);
  SANDBOX_END;

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 2);
  if (ms != 2)
    push_info_msg(_("You can use only two call to malloc to complete the task"));

  // check if new element is malloced
  int mal = malloced((void*) stack);
  CU_ASSERT_TRUE(mal);
  // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
  if (mal){
    //printf("src = %s, name = %s\n", src, stack->name);
    CU_ASSERT_STRING_EQUAL(src, stack->name);
    if (!strcmp(src, stack->name)){
      char tmp[100];
      sprintf(tmp, _("The pushed value differ from the expected one\nWaited : %s\nReceived : %s\n"), src, stack->name);
      push_info_msg(tmp);
    }

  }
  else
    push_info_msg(_("The returned pointer is not malloced"));

  if (!stack && !stack->next){
    CU_FAIL("The head node is not correctly updated");
    return;
  }

  // add src to the array stack
  *(a+6) = src;

  /*print_array_as_stack(a ,7);
  print_stack(stack , 7);
  printf("%i\n", ret);*/

  // if correct struct, @stru = 0
  int stru =check_stack(stack, a, 7);
  CU_ASSERT_TRUE(!stru);
  if (stru)
    push_info_msg(_("The structure of the stack has changed"));

  // check the return value of the function
  CU_ASSERT_TRUE(!ret);
  if (ret)
    push_info_msg(_("The function return an error code in normal case"));

  free_stack(stack);

}

/*void test_push_empty(){
  set_test_metadata("push", _("Check if the insertion of a new node into an empty list"), 1);

  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return;

  head->next = NULL;
  head->value = 0;

 int ret;

 srandom(646415654);

 SANDBOX_BEGIN;
 ret = push(&head, random());
 SANDBOX_END;

 CU_ASSERT_EQUAL(ret, 0);
 if (!ret)
  push_info_msg(_("You detected an error in normal case"));



  free_stack(head);
}

void test_pop_empty(){

  set_test_metadata("pop", _("Check the beahvior of the function when popping an empty stack"), 1);

  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return;

  head->next = NULL;
  head->value = 0;

  int ret;

  srandom(6464898);

  SANDBOX_BEGIN;
  ret = pop(&head, random());
  SANDBOX_END;

  CU_ASSERT_TRUE(ret);
  if (!ret)
    push_info_msg(_("You did not detect"));



}*/

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_push_param_nomem, test_push_changing_param, test_push_general/*, test_push_general, test_push_empty*/);
}
