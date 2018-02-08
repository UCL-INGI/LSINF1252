// CTester template

#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"
#include "CTester/CTester.h"


void test_sum2(){
  set_test_metadata("version1", _("Check the `result` value"), 1);

  SANDBOX_BEGIN;
  version1(3, 7);
  SANDBOX_END;

  CU_ASSERT_EQUAL(result, 10);
  if (result != 10){
    char msg[80];
    sprintf(msg, _("Expected value: 10. Received value: %i"), result);
    push_info_msg(msg);
  }

  SANDBOX_BEGIN;
  version1(136, 3);
  SANDBOX_END;

  CU_ASSERT_EQUAL(result, 139);
  if (result != 139){
    char msg[80];
    sprintf(msg, _("Expected value: 139. Received value: %i"), result);
    push_info_msg(msg);
  }
}


void test_sum3(){
  set_test_metadata("version2", _("Check the value of @r"), 1);

  int a = 1, b = 8, r=-1;
  int a2 = 10, b2 = 88, r2=-1;


  SANDBOX_BEGIN;
  version2(&a, &b, &r);
  version2(&a2, &b2, &r2);
  SANDBOX_END;

  CU_ASSERT_EQUAL(r, a+b);
  CU_ASSERT_EQUAL(r2, a2+b2);
  if (r != a+b){
    char msg[80];
    sprintf(msg, _("Expected value: %d. Received value: %i"), a+b,r);
    push_info_msg(msg);
  }
  if (r2 != a2+b2){
    char msg[80];
    sprintf(msg, _("Expected value: %d. Received value: %i"), a2+b2,r2);
    push_info_msg(msg);
  }
}


int main(int argc,char** argv)
{
    BAN_FUNCS(set_tag);
    RUN(test_sum2, test_sum3);
}
