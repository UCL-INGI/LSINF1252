#include "matchers.h"

static int anything_try(CXCursor cursor, void *data,
                        matcher_captures *captures) { return 1; }
static void anything_clean_up(void *data) {}
matcher match_anything(void) {
  return (matcher){anything_try, anything_clean_up, NULL};
}
