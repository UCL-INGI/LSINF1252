#include "matchers.h"

static int pointer_try(CXCursor cursor, void *data,
                       matcher_captures *captures) {
  switch (clang_getCursorType(cursor).kind) {
  case CXType_Pointer:
  case CXType_DependentSizedArray:
  case CXType_VariableArray:
  case CXType_IncompleteArray:
  case CXType_ConstantArray:
  case CXType_NullPtr:
    return 1;
  default: return 0;
  }
}

static void pointer_clean_up(void *data) {}

matcher match_pointer(void) {
  return (matcher){pointer_try, pointer_clean_up, NULL};
}
