// CTester template

#include <stdlib.h>
#include "CTester/CTester.h"
#include "student_code.h"

void test_my_strlen(){
  set_test_metadata("my_strlen", _("Check the return value of the function"), 2);

  int ret;

  char *s = "26fgb.thjfgjppzaerofa,zprjgfbl";

  SANDBOX_BEGIN;
  ret = my_strlen(s);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret, 30);
  if (ret != 30)
    push_info_msg(_("The function return the wrong value"));

  char *s2 = "k26fgb.thjfgjpp";

  SANDBOX_BEGIN;
  ret = my_strlen(s2);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret, 15);
  if (ret != 15)
    push_info_msg(_("The function return the wrong value"));

  char *s3 = "";
  SANDBOX_BEGIN;
  ret = my_strlen(s3);
  SANDBOX_END;

  CU_ASSERT_EQUAL(ret, 0);
  if (ret != 0)
    push_info_msg(_("The function return the wrong value"));


}

int main(int argc,char** argv)
{
    BAN_FUNCS(strlen,strnlen);
    RUN(test_my_strlen);
}
