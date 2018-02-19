#include "matchers.h"
#include "utils.h"
#include <stdlib.h>

typedef struct binop_data {
  int op;
  matcher lhs, rhs;
} binop_data;

static int binop_try(CXCursor cursor, void *data, matcher_captures *captures) {
  binop_data *arg = data;
  if (!matcher_check_binop(cursor, arg->op))
    return 0;

  CXCursor lhs, rhs;
  matcher_extract_binop(cursor, &lhs, &rhs);

  return matcher_try(arg->lhs, lhs, captures) &&
    matcher_try(arg->rhs, rhs, captures);
}

static void binop_clean_up(void *data) {
  binop_data *arg = data;

  matcher_release(arg->lhs);
  matcher_release(arg->rhs);
  free(arg);
}

matcher match_binop(int op, matcher lhs, matcher rhs) {
  binop_data *data = malloc(sizeof(*data));
  data->op = op;
  data->lhs = match_result(lhs);
  data->rhs = match_result(rhs);

  return (matcher){binop_try, binop_clean_up, data};
}
