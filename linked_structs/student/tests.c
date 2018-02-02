// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"


struct node *generate_list(){
  int len = sizeof(struct node);
  int i, j;

  struct node *head, *tmp, *act;

  for (j = 0; j < 10; j++){
    tmp = (struct node*) malloc(len);
    for(i = 0; i < len; i++){
        if (!i%2)
          *(tmp+i) = (void*) 0;
        else
          *(tmp+i) = j+1;
    }

    if (j == 0)
      head = tmp;
    else
      act->next = tmp;

    act = tmp;
  }

  act->next = NULL;

  return head;
}

void test_pair_filter(){
  set_test_metadata("filter", _("Test"), 1);
  int len = sizeof(struct node);

  struct node* head = generate_list(), ret;

  SANDBOX_BEGIN;
  ret = pair_filter(head);
  SANDBOX_END;

  struct node *run1 = ret, *run2 = head;
  int cmp;
  while(!run){
    cmp = memcmp((const void*) run1, (const void*) run2, len);
    if (cmp != 0){
      CU_FAIL("The function produced a wrong list");
      push_info_msg(_("The function produced a wrong list"));
      return;
    }

    run1 = run1->next;
    run2 = run2->next;
    if (!run2->next)
      break;
    run2 = run2->next;
  }
  CU_PASS("The produced list match");
}


int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_myfunc_ret);
}
