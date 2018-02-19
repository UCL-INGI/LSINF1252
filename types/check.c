#include <matchers.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CXCursor matched_cursor;
int matched = 0;

void on_match(CXCursor cursor,
              const char *filename,
              unsigned int line, unsigned int column,
              const matcher_captures *captures, void *data) {
  if (!matched) {
    matched_cursor = cursor;
    matched = 1;
  }
}

void print_error(int qid) {
  char command[2048];
  snprintf(command, 2048,
           "feedback --result failed "
           "--feedback 'Erreur à la question %d'", qid);
  system(command);
}

int check_type(CXCursor cursor, const char *type, int qid) {
  CXString str = clang_getTypeSpelling(
  clang_getCanonicalType(clang_getCursorType(cursor)));
  int res = strcmp(clang_getCString(str), type) == 0;
  clang_disposeString(str);

  if (!res) print_error(qid);

  return res;
}

int check_keyword(CXCursor cursor, const char *keyword, int qid) {
  if (!matcher_has_token(cursor, keyword)) {
    print_error(qid);
    return 0;
  }
  else
    return 1;
}

int check_q1(CXCursor cursor);
int check_q2(CXCursor cursor);
int check_q4(CXCursor cursor);
int check_q7(CXCursor cursor);

int try_question(const char *filename, matcher m, int (*check)(CXCursor)) {
  const char *args[] = {MatcherClangIncludePath};
  matcher_file f = matcher_open_with_diagnoses(filename, 1, args, 1);
  matched = 0;

  int result;

  result = clang_getNumDiagnostics(f.tu) == 0;
  if (clang_getNumDiagnostics(f.tu) > 0) {
    fprintf(stderr, "Votre programme ne compile pas.\n");
    result = 0;
  }
  else if (!matcher_look_for(f, m, on_match, NULL)) {
    fprintf(stderr, "Pas de variable x trouvée dans %s.\n", filename);
    result = 0;
  }
  else
    result = check(matched_cursor);

  matcher_close_file(f);
  return result ? 0 : 1;
}

int main(int argc, char **argv) {
  int status = 0;

  matcher m = match_var_decl("x");

  if ((status = try_question("q1.c", m, check_q1)) != 0) goto on_failure;
  if ((status = try_question("q2.c", m, check_q2)) != 0) goto on_failure;
  if ((status = try_question("q4.c", m, check_q4)) != 0) goto on_failure;
  if ((status = try_question("q7.c", m, check_q7)) != 0) goto on_failure;

on_failure: matcher_release(m);

  return status;
}

int check_q1(CXCursor cursor) {
  return check_type(cursor, "int", 1);
}

int check_q2(CXCursor cursor) {
  return check_type(cursor, "int [10]", 2);
}

int check_q4(CXCursor cursor) {
  return check_type(cursor, "int *", 4);
}

int check_q7(CXCursor cursor) {
  return check_type(cursor, "int [8][5]", 7);
}
