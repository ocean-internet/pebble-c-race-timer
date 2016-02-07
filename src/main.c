#include <pebble.h>
#include "main.h"

static Window *s_main_window;

int main(void) {
  init();
  app_event_loop();
  deinit();
}

static void init() {}

static void deinit() {}