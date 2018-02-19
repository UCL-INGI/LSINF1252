#include "matchers.h"
#include "utils.h"
#include <stdlib.h>

static int result_try(CXCursor cursor, void *data,
                      matcher_captures *captures) {
  return matcher_follow_reference(cursor, data, captures);
}

static void result_clean_up(void *data) {
  matcher_release(*(matcher*)data);
  free(data);
}

matcher match_result(matcher m) {
  matcher *data = malloc(sizeof(m));
  *data         = m;
  return (matcher){result_try, result_clean_up, data};
}
