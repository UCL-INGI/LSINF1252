#include "matchers.h"
#include <stdlib.h>

void matcher_init_captures(matcher_captures *captures) {
  captures->size  = 0;
  captures->array = NULL;
}

void matcher_release_captures(matcher_captures *captures) {
  free(captures->array);
}

void matcher_reset_captures(matcher_captures *captures) {
  if (!captures) return;
  for (size_t i = 0; i < captures->size; i++)
    captures->array[i].is_set = 0;
}

int matcher_set_capture(matcher_captures *captures, int capture_id,
                        CXCursor cursor) {
  if (!captures) return 0;

  if (0 <= capture_id && (size_t)capture_id < captures->size) {
    captures->array[capture_id].is_set = 1;
    captures->array[capture_id].cursor = cursor;
    return 0;
  }
  else if (capture_id < 0)
    return -1;
  else {
    struct matcher_captured_node *new_buffer =
      realloc(captures->array, (capture_id+1)*sizeof(*new_buffer));
    if (new_buffer) {
      for (int i = captures->size; i < capture_id; i++)
        new_buffer[i].is_set = 0;
      new_buffer[capture_id].is_set = 1;
      new_buffer[capture_id].cursor = cursor;

      captures->size = capture_id + 1;
      captures->array = new_buffer;

      return 0;
    }
    else
      return -1;
  }
}

int matcher_get_capture(const matcher_captures *captures, int capture_id,
                        CXCursor *out_cursor) {
  if (!captures) return 0;

  if (0 <= capture_id && (size_t)capture_id < captures->size) {
    if (captures->array[capture_id].is_set) {
      if (out_cursor)
        *out_cursor = captures->array[capture_id].cursor;
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}
