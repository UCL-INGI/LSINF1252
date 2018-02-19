#include "utils.h"
#include "matchers.h"
#include "globals.h"
#include <string.h>

int matcher_has_token(CXCursor cursor, const char *op) {
  int result = 0;

  CXSourceRange range = clang_getCursorExtent(cursor);
  CXToken *tokens;
  unsigned int token_count;
  clang_tokenize(matcher_tu, range, &tokens, &token_count);

  for (unsigned i = 0; i < token_count && !result; i++) {
    CXCursor a = clang_getCursor(matcher_tu,
                                 clang_getTokenLocation(matcher_tu, tokens[i]));
    if (clang_equalRanges(range, clang_getCursorExtent(a))) {
      CXString code = clang_getTokenSpelling(matcher_tu, tokens[i]);
      if (strcmp(clang_getCString(code), op) == 0) result = 1;
      clang_disposeString(code);
    }
  }
  clang_disposeTokens(matcher_tu, tokens, token_count);

  return result;
}

int matcher_is_binop(CXCursor binop, const char *op) {
  return clang_getCursorKind(binop) == CXCursor_BinaryOperator &&
    matcher_has_token(binop, op);
}

int matcher_check_binop(CXCursor binop, int op) {
  if (op & MatcherOpAdd    && matcher_is_binop(binop, "+"))  return 1;
  if (op & MatcherOpSub    && matcher_is_binop(binop, "-"))  return 1;
  if (op & MatcherOpMul    && matcher_is_binop(binop, "*"))  return 1;
  if (op & MatcherOpDiv    && matcher_is_binop(binop, "/"))  return 1;
  if (op & MatcherOpMod    && matcher_is_binop(binop, "%"))  return 1;
  if (op & MatcherOpLT     && matcher_is_binop(binop, "<"))  return 1;
  if (op & MatcherOpGT     && matcher_is_binop(binop, ">"))  return 1;
  if (op & MatcherOpLTE    && matcher_is_binop(binop, "<=")) return 1;
  if (op & MatcherOpGTE    && matcher_is_binop(binop, ">=")) return 1;
  if (op & MatcherOpEq     && matcher_is_binop(binop, "==")) return 1;
  if (op & MatcherOpNeq    && matcher_is_binop(binop, "!=")) return 1;
  if (op & MatcherOpLShift && matcher_is_binop(binop, "<<")) return 1;
  if (op & MatcherOpRShift && matcher_is_binop(binop, ">>")) return 1;
  if (op & MatcherOpAssign && matcher_is_binop(binop, "=")) return 1;

  return 0;
}

typedef struct cond_expr {
  int i;

  CXCursor cond;
  CXCursor if_true, if_false;
} cond_expr;

static enum CXChildVisitResult visit_cond_expr(CXCursor  cursor,
                                               CXCursor  parent,
                                               CXClientData data) {
  cond_expr *expr = (cond_expr*)data;
  switch (expr->i++) {
  case 0: expr->cond     = cursor; break;
  case 1: expr->if_true  = cursor; break;
  case 2: expr->if_false = cursor; break;
  }
  return CXChildVisit_Continue;
}


void matcher_extract_cond_expr(
  CXCursor cursor, CXCursor *cond, CXCursor *if_true, CXCursor *if_false) {
  cond_expr expr;
  expr.i = 0;

  clang_visitChildren(cursor, visit_cond_expr, (CXClientData)&expr);

  if (cond) *cond         = expr.cond;
  if (if_true) *if_true   = expr.if_true;
  if (if_false) *if_false = expr.if_false;
}

typedef struct binop_arg {
  int i;
  CXCursor lhs, rhs;
} binop_arg;

static enum CXChildVisitResult visit_binop(CXCursor  cursor,
                                           CXCursor  parent,
                                           CXClientData data) {
  binop_arg *arg = data;
  if (arg->i++ == 0) arg->lhs = cursor;
  else arg->rhs = cursor;

  return CXChildVisit_Continue;
}

void matcher_extract_binop(CXCursor cursor, CXCursor *lhs, CXCursor *rhs) {
  binop_arg arg;
  arg.i = 0;

  clang_visitChildren(cursor, visit_binop, &arg);
  if (lhs) *lhs = arg.lhs;
  if (rhs) *rhs = arg.rhs;
}

typedef struct  var_decl {
  int i;
  CXCursor type;
  CXCursor value;
} var_decl;

static enum CXChildVisitResult visit_var_decl(CXCursor  cursor,
                                              CXCursor  parent,
                                              CXClientData data) {
  var_decl *arg = data;
  if (arg->i++ == 0) arg->type = cursor;
  else {
    arg->value = cursor;
    return CXChildVisit_Break;
  }

  return CXChildVisit_Continue;
}

int matcher_extract_var_decl(CXCursor cursor, CXCursor *type, CXCursor *value) {
  var_decl decl;
  decl.i = 0;

  int ret = clang_visitChildren(cursor, visit_var_decl, &decl);
  if (type) *type = decl.type;
  if (value) *value = decl.value;

  return ret;
}

typedef struct rec_data {
  matcher *m;
  matcher_captures *captures;
} rec_data;

static
enum CXChildVisitResult rec_follow_reference(CXCursor cursor, CXCursor parent,
                                             CXClientData data) {
  rec_data *arg = data;
  if (matcher_follow_reference(cursor, arg->m, arg->captures))
    return CXChildVisit_Break;
  else
    return CXChildVisit_Continue;
}

int matcher_follow_reference(CXCursor cursor, matcher *m,
                             matcher_captures *captures) {
  if (matcher_try(*m, cursor, captures)) return 1;

  switch (clang_getCursorKind(cursor)) {
  case CXCursor_ConditionalOperator: {
    CXCursor if_true, if_false;
    matcher_extract_cond_expr(cursor, NULL, &if_true, &if_false);
    return matcher_follow_reference(if_true, m, captures) ||
      matcher_follow_reference(if_false, m, captures);
    break;
  }
  case CXCursor_BinaryOperator: {
    if (matcher_is_binop(cursor, "=")) {
      binop_arg arg;
      arg.i = 0;
      clang_visitChildren(cursor, visit_binop, &arg);
      return matcher_follow_reference(arg.rhs, m, captures);
    }
    else
      return 0;
  }
  case CXCursor_ParenExpr:
  case CXCursor_CStyleCastExpr:
  case CXCursor_CXXStaticCastExpr:
  case CXCursor_CXXDynamicCastExpr:
  case CXCursor_CXXReinterpretCastExpr:
  case CXCursor_CXXConstCastExpr:
  case CXCursor_CXXFunctionalCastExpr:
  case CXCursor_UnexposedExpr: {
    rec_data data = {m, captures};
    return clang_visitChildren(cursor, rec_follow_reference, &data);
  }
  default: return 0;
  }
}
