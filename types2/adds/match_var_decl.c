#include "matchers.h"
#include <string.h>

static
int var_decl_try(CXCursor cursor, void *data, matcher_captures *captures) {
  const char *name = data;
  if (clang_getCursorKind(cursor) != CXCursor_VarDecl)
    return 0;

  CXString spelling = clang_getCursorSpelling(cursor);
  int same_name = strcmp(clang_getCString(spelling), name) == 0;
  clang_disposeString(spelling);

  return same_name;
}

static
void var_decl_clean_up(void *data) {}

matcher match_var_decl(char *name) {
  return (matcher){var_decl_try, var_decl_clean_up, name};
}
