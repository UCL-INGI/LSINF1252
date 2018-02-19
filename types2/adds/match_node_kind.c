#include "matchers.h"
#include <stdlib.h>

static int try_node_kind(CXCursor cursor, void *data,
                         matcher_captures *captures) {
  return clang_getCursorKind(cursor) == *(enum CXCursorKind*)data;
}

matcher match_node_kind(enum CXCursorKind kind) {
  enum CXCursorKind *data = malloc(sizeof(*data));
  *data = kind;

  return (matcher){try_node_kind, free, data};
}

matcher match_array_subscript(void) {
  return match_node_kind(CXCursor_ArraySubscriptExpr);
}
