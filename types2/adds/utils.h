#ifndef UTILS_H_
#define UTILS_H_

#include <clang-c/Index.h>
#include "matchers.h"

int matcher_has_token(CXCursor cursor, const char *op);
int matcher_is_binop(CXCursor binop, const char *op);
int matcher_check_binop(CXCursor binop, int op_mask);

void matcher_extract_cond_expr(
  CXCursor cursor, CXCursor *cond, CXCursor *if_true, CXCursor *if_false);
void matcher_extract_binop(CXCursor cursor, CXCursor *lhs, CXCursor *rhs);
int matcher_extract_var_decl(CXCursor cursor, CXCursor *type, CXCursor *value);

int matcher_follow_reference(CXCursor cursor, matcher *m,
                             matcher_captures *captures);

#endif
