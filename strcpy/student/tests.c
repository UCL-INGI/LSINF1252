// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_strcpy_return() {
  set_test_metadata("strcpy_impl", _("Check the returned pointer in normal case"), 1);

  char *ret = NULL;
  const char *src = "Chaine de char de test un peu courte mais pas trop quand meme";

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = buf_strcpy(src);
  SANDBOX_END;

  // Tests
  //-----------------------------------------------------------------

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 1);
  if (ms > 1)
    push_info_msg(_("You used more than one call to malloc"));

  // check if new element is malloced
  int mal = malloced((void*) ret);
  CU_ASSERT_TRUE(mal);
  // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
  if (mal){
      CU_ASSERT_EQUAL(ret[strlen(src)],'\0');
      if (ret[strlen(src)== '\0']){
          CU_ASSERT_STRING_EQUAL(ret, src);
          if (strncmp(ret, src, 62) != 0){
              char msg[80];
              sprintf(msg, _("The strings are different.\nExpected string: %s. Received value: %s"), src, ret);
              push_info_msg(msg);
          }
      }
      else
          push_info_msg(_("The returned pointer is does not finish by \0"));
  }
  else
    push_info_msg(_("The returned pointer is not malloced"));

free(ret);

}

void test_strcpy_nomem() {
  set_test_metadata("strcpy_impl", _("Check the behavior of the function when the call to malloc fails"), 1);

  char *ret = NULL;
  char *src = "Chaine de char de test un peu courte mais pas trop quand meme";

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;

  SANDBOX_BEGIN;
  ret = buf_strcpy(src);
  SANDBOX_END;

  CU_ASSERT_PTR_NULL(ret);
  if (ret)
    push_info_msg(_("The return value of your implementation is wrong"));

  free(ret);

}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_strcpy_return, test_strcpy_nomem);
}
