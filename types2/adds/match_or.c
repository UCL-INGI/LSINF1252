#include "matchers.h"
#include <stdlib.h>

typedef struct or_data {
  matcher a, b;
} or_data;

static int or_try(CXCursor cursor, void *data, matcher_captures *captures) {
  or_data *arg = data;
  return matcher_try(arg->a, cursor, captures) ||
    matcher_try(arg->b, cursor, captures);
}

static void or_clean_up(void *data) {
  or_data *arg = data;
  matcher_release(arg->a);
  matcher_release(arg->b);
  free(arg);
}

matcher match_or(matcher a, matcher b) {
  or_data *data = malloc(sizeof(*data));
  data->a = a;
  data->b = b;
  return (matcher){or_try, or_clean_up, data};
}
