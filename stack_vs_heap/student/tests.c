// CTester template

#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"
#include "CTester/CTester.h"

/*void test_retour1() {


  CU_ASSERT_EQUAL(retour1(), 5);

}*/

void test_sum2(){
  set_test_metadata("version1", _("Check the `result` value"), 1);

  SANDBOX_BEGIN;
  version1(3, 7);
  SANDBOX_END;

  CU_ASSERT_EQUAL(result, 10);
  if (result != 10){
    char msg[80];
    sprintf(msg, "Expected value: 10. Received value: %i", result);
    push_info_msg(_(msg));
  }
}


void test_sum3(){
  set_test_metadata("version2", _("Check the value of @a"), 1);

  int a = 1, b = 8;

  SANDBOX_BEGIN;
  version2(&a, &b);
  SANDBOX_END;

  CU_ASSERT_EQUAL(a, 9);
  if (a != 9){
    char msg[80];
    sprintf(msg, "Expected value: 9. Received value: %i", result);
    push_info_msg(_(msg));
  }
}


int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_sum2, test_sum3);
}
