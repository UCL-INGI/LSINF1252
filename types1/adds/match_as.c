#include "matchers.h"
#include <stdlib.h>

typedef struct capture_data {
  int capture_id;
  matcher m;
} capture_data;

static int capture_try(CXCursor cursor, void *data,
                       matcher_captures *captures) {
  capture_data *arg = data;
  if (matcher_try(arg->m, cursor, captures)) {
    matcher_set_capture(captures, arg->capture_id, cursor);
    return 1;
  }
  else
    return 0;
}

static void capture_clean_up(void *data) {
  matcher_release(((capture_data*)data)->m);
  free(data);
}

matcher match_as(int capture_id, matcher m) {
  capture_data *data = malloc(sizeof(*data));
  data->capture_id = capture_id;
  data->m = m;
  return (matcher){capture_try, capture_clean_up, data};
}
