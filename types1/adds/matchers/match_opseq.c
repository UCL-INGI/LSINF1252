#include "matchers.h"
#include "globals.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

typedef struct opseq_arg {
  int op;

  size_t size, capa;
  CXCursor *buffer;
} opseq_arg;

static int opseq_arg_init(opseq_arg *arg, int op) {
  arg->op = 0;

  arg->buffer = malloc(sizeof(*arg->buffer) * 2);
  arg->size   = 0;
  arg->capa   = 2;

  return arg->buffer != NULL;
}

static int opseq_arg_push(opseq_arg *arg, CXCursor cursor) {
  if (arg->size == arg->capa) {
    CXCursor *data = realloc(arg->buffer, sizeof(*arg->buffer) *
                             arg->capa * 3/2);
    if (!data) return 0;

    arg->buffer = data;
    arg->capa   = arg->capa * 3/2;
  }

  arg->buffer[arg->size++] = cursor;
  return 1;
}

enum CXChildVisitResult visit_opseqs(CXCursor  cursor, CXCursor parent,
                                     CXClientData data) {
  opseq_arg *arg = data;

  if (matcher_check_binop(cursor, arg->op))
    return CXChildVisit_Recurse;
  else {
    opseq_arg_push(arg, cursor);
    return CXChildVisit_Continue;
  }
}

int extract_arguments(int op, CXCursor cursor,
                      size_t *size, CXCursor **args) {
  opseq_arg arg;

  if (!matcher_check_binop(cursor, op))
    return 0;

  if (!opseq_arg_init(&arg, op)) return 0;
  clang_visitChildren(cursor, visit_opseqs, &arg);

  *size = arg.size;
  *args = arg.buffer;

  return 1;
}

typedef struct opseq_data {
  int     op;
  matcher a, b;
} opseq_data;

static void opseq_clean_up(void *data) {
  opseq_data *arg = (opseq_data*)data;
  matcher_release(arg->a);
  matcher_release(arg->b);
  free(arg);
}

static int opseq_try(CXCursor  cursor, void *data, matcher_captures *captures) {
  opseq_data *arg = (opseq_data*)data;

  size_t    size;
  CXCursor *args;

  if (extract_arguments(arg->op, cursor, &size, &args)) {
    /*
     * This matches if:
     *   - one of the argument matched by a and
     *   - there is another, separate argument matched by b
     *
     * This holds if we've found at least one occurence of either argument and
     * if
     *   - we've seen an argument matched by a but not b (or the other way
     *     around) or
     *   - we've seen at least one of them more than once
     */
    int seen_a = 0, seen_b = 0, seen_a_xor_b = 0;
    for (size_t i = 0; i < size; i++) {
      int has_a = matcher_follow_reference(args[i], &arg->a, captures);
      int has_b = matcher_follow_reference(args[i], &arg->b, captures);

      if (has_a) seen_a += 1;
      if (has_b) seen_b += 1;
      if (has_a ^ has_b) seen_a_xor_b = 1;
    }

    free(args);

    return seen_a && seen_b &&
      (seen_a_xor_b || seen_a > 1 || seen_b > 1);
  }
  else
    return 0;
}

matcher match_opseq(int op, matcher a, matcher b) {
  opseq_data *data = malloc(sizeof(*data));
  data->op = op;
  data->a = a;
  data->b = b;

  return (matcher){opseq_try, opseq_clean_up, data};
}
