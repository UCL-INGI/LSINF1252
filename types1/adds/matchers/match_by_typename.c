#include "matchers.h"
#include <stdlib.h>
#include <string.h>

typedef struct typename_data {
  const char *name;
} typename_data;

static int typename_try(CXCursor cursor, void *data,
                        matcher_captures *captures) {
  typename_data *arg = data;

  if (!clang_isExpression(clang_getCursorKind(cursor)))
    return 0;

  CXString type = clang_getTypeSpelling(
    clang_getCanonicalType(clang_getCursorType(cursor)));
  int ret = strcmp(clang_getCString(type), arg->name) == 0;
  clang_disposeString(type);

  return ret;
}

matcher match_by_typename(const char *name) {
  typename_data *data = malloc(sizeof(*data));
  data->name = name;

  return (matcher){typename_try, free, data};
}
