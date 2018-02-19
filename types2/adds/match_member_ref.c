#include "matchers.h"
#include "globals.h"
#include <stdlib.h>
#include <string.h>

typedef struct member_ref_data {
  matcher src;
  const char *field_name;
} member_ref_data;

typedef struct member_ref_arg {
  size_t depth;
  CXCursor cursor;
} member_ref_arg;

static enum CXChildVisitResult visit_member_ref(
  CXCursor cursor, CXCursor parent,
  CXClientData data) {
  member_ref_arg *arg = data;
  if (arg->depth == 0) {
    arg->depth++;
    return CXChildVisit_Recurse;
  }
  else {
    arg->cursor = cursor;
    return CXChildVisit_Break;
  }
}

static void extract_member_ref(CXCursor cursor, CXCursor *arg) {
  member_ref_arg visit_arg;
  visit_arg.depth = 0;

  clang_visitChildren(cursor, visit_member_ref, &visit_arg);
  if (arg) *arg = visit_arg.cursor;
}

static int member_ref_try(CXCursor cursor, void *data,
                          matcher_captures *captures) {
  member_ref_data *arg = data;

  if (clang_getCursorKind(cursor) != CXCursor_MemberRefExpr) return 0;

  CXString field_name = clang_getCursorSpelling(cursor);
  int same_field = strcmp(clang_getCString(field_name), arg->field_name) == 0;
  clang_disposeString(field_name);

  if (!same_field) return 0;

  CXCursor src;
  extract_member_ref(cursor, &src);

  return matcher_try(arg->src, src, captures);
}

static void member_ref_clean_up(void *data) {
  member_ref_data *arg = data;
  matcher_release(arg->src);
  free(arg);
}

matcher match_member_ref(matcher src, const char *field_name) {
  member_ref_data *data = malloc(sizeof(*data));
  data->src = src;
  data->field_name = field_name;
  return (matcher){member_ref_try, member_ref_clean_up, data};
}
