#include "matchers.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

typedef struct call_data {
  const char *name;

  size_t n;
  matcher *args;
} call_data;

typedef struct call_args {
  size_t n, i;
  size_t depth;

  CXCursor function;
  CXCursor *args;
} call_args;

static enum CXChildVisitResult visit_call(CXCursor cursor, CXCursor parent,
                                          CXClientData data) {
  call_args *args = (call_args*)data;

  if (args->depth++ == 0)
    return CXChildVisit_Recurse;
  else {
    if (args->i++ == 0)
      args->function = cursor;
    else
      args->args[args->i - 2] = cursor;

    return CXChildVisit_Continue;
  }
}

static void extract_call_arguments(CXCursor cursor, size_t n,
                                   CXCursor *function,
                                   size_t *out_n, CXCursor **args) {
  call_args data;
  data.i = 0;
  data.n = n;
  data.depth = 0;
  data.args = malloc(sizeof(*data.args) * n);

  clang_visitChildren(cursor, visit_call, &data);

  if (function) *function = data.function;
  if (out_n) *out_n = data.i - 1;
  if (args)  *args = data.args;
  else free(data.args);
}

static int call_try(CXCursor cursor, void *data, matcher_captures *captures) {
  call_data *matcher_data = (call_data*)data;

  if (clang_getCursorKind(cursor) != CXCursor_CallExpr) return 0;

  CXString name = clang_getCursorSpelling(cursor);
  int same_name = strcmp(clang_getCString(name), matcher_data->name) == 0;
  clang_disposeString(name);

  if (!same_name) return 0;

  CXCursor *args;
  size_t n;
  extract_call_arguments(cursor, matcher_data->n, NULL, &n, &args);

  if (n != matcher_data->n) {
    free(args);
    return 0;
  }

  size_t i;
  for (i = 0; i < n; i++) {
    if (!matcher_try(matcher_data->args[i], args[i], captures)) break;
  }

  free(args);

  return i == n;
}

static void call_clean_up(void *data) {
  call_data *matcher_data = (call_data*)data;
  for (size_t i = 0; i < matcher_data->n; i++)
    matcher_release(matcher_data->args[i]);
  free(matcher_data->args);
  free(matcher_data);
}

matcher match_call(const char *name, size_t n, matcher *args) {
  call_data *data = malloc(sizeof(*data));

  data->name = name;
  data->n = n;

  data->args = malloc(sizeof(*data->args) * n);
  for (size_t i = 0; i < n; i++) data->args[i] = match_result(args[i]);

  return (matcher){call_try, call_clean_up, data};
}
