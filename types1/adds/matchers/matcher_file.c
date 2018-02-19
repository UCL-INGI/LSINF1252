#include "matchers.h"

matcher_file matcher_open(const char *filename, int argc, const char *argv[]) {
  return matcher_open_with_diagnoses(filename, argc, argv, 0);
}

matcher_file matcher_open_with_diagnoses(const char *filename,
                                         int argc, const char *argv[],
                                         int print_diagnoses) {
  matcher_file file;

  file.index = clang_createIndex(0, print_diagnoses);
  file.tu = clang_parseTranslationUnit(file.index, filename, argv, argc, NULL,
                                       0, CXTranslationUnit_None);

  return file;
}

void matcher_close_file(matcher_file f) {
  clang_disposeTranslationUnit(f.tu);
  clang_disposeIndex(f.index);
}
