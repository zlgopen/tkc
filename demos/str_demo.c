#include "tkc.h"

int main(void) {
  str_t str;

  str_init(&str, 100);
  str_append_more(&str, "hello ", "world", NULL);
  log_debug("%s\n", str.str);
  str_reset(&str);
  return RET_OK;
}
