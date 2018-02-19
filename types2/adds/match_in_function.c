#include "matchers.h"
#include <stdlib.h>
#include <string.h>

typedef struct in_function_data {
  const char *name;
  matcher m;
} in_function_data;

typedef struct sub_function_data {
  matcher m;
  matcher_captures *captures;
} sub_function_data;

static enum CXChildVisitResult in_function_visit_children(CXCursor cursor,
                                                          CXCursor parent,
                                                          CXClientData data) {
  sub_function_data *arg = data;
  if (matcher_try(arg->m, cursor, arg->captures))
    return CXChildVisit_Break;
  else
    return CXChildVisit_Recurse;
}

static int in_function_try(CXCursor cursor, void *data,
                           matcher_captures *captures) {
  in_function_data *parsed_data = data;

  if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
    CXString name = clang_getCursorSpelling(cursor);
    int same_function = strcmp(clang_getCString(name), parsed_data->name) == 0;
    clang_disposeString(name);

    if (same_function) {
      sub_function_data arg = {parsed_data->m, captures};
      return clang_visitChildren(cursor, in_function_visit_children, &arg);
    }
    else
      return 0;
  }
  else
    return 0;
}

static void in_function_clean_up(void *data) {
  in_function_data *parsed_data = data;
  matcher_release(parsed_data->m);
  free(parsed_data);
}

matcher match_in_function(const char *name, matcher m) {
  in_function_data *data = malloc(sizeof(*data));
  data->name = name;
  data->m = m;

  return (matcher){in_function_try, in_function_clean_up, data};
}
