#include "matchers.h"
#include "globals.h"
#include <string.h>

static int sizeof_try(CXCursor cursor, void *data,
                      matcher_captures *captures) {
  if (clang_getCursorKind(cursor) != CXCursor_UnexposedExpr)
    return 0;
  return matcher_has_token(cursor, "sizeof");
}

static void sizeof_clean_up(void *data) {}

matcher match_sizeof(void) {
  return (matcher){sizeof_try, sizeof_clean_up, NULL};
}
