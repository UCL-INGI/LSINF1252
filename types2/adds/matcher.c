#include "matchers.h"
#include "globals.h"

CXTranslationUnit matcher_tu;

typedef struct client_data {
  matcher matcher;
  matcher_visitor visitor;
  matcher_captures captures;
  void *user_data;
  int found;
} client_data;

static enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                       CXClientData data);

int matcher_try(matcher m, CXCursor cursor, matcher_captures *captures) {
  return m.run(cursor, m.data, captures);
}

void matcher_release(matcher m) {
  m.on_clean_up(m.data);
}

void matcher_use_file(matcher_file file) {
  matcher_tu = file.tu;
}

int matcher_look_for(matcher_file file, matcher m, matcher_visitor to_visit,
                     void *user_data) {
  matcher_use_file(file);

  matcher_captures captures;
  matcher_init_captures(&captures);

  client_data data = {m, to_visit, captures, user_data, 0};
  clang_visitChildren(clang_getTranslationUnitCursor(matcher_tu), visitor,
                      &data);

  matcher_release_captures(&captures);

  return data.found;
}

static enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                       CXClientData data) {
  client_data *parsed_data = data;

  if (matcher_try(parsed_data->matcher, cursor, &parsed_data->captures)) {
    parsed_data->found = 1;
    if (parsed_data->visitor) {
      CXSourceLocation loc = clang_getCursorLocation(cursor);
      CXFile file;
      unsigned line, column;
      clang_getFileLocation(loc, &file, &line, &column, NULL);
      CXString filename = clang_getFileName(file);
      parsed_data->visitor(cursor, clang_getCString(filename), line, column,
                           &parsed_data->captures, parsed_data->user_data);
      clang_disposeString(filename);
    }
  }

  return CXChildVisit_Recurse;
}
