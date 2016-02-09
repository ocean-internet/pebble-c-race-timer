#include <pebble.h>
#include "menu/main-menu.h"

static Window *s_mainWindow;

static void init();
static void deinit();
static void mainWindowLoad(Window *window);
static void mainWindowUnload(Window *window);

int main(void) {
  init();
  app_event_loop();
  deinit();
}

static void init() {
    
    s_mainWindow = window_create();
    window_set_window_handlers(s_mainWindow, (WindowHandlers) {
        .load   = mainWindowLoad,
        .unload = mainWindowUnload
    });
    window_stack_push(s_mainWindow, true);
}

static void deinit() {
    
    window_destroy(s_mainWindow);
}

static void mainWindowLoad(Window *window) {
    
    Layer *windowLayer = window_get_root_layer(window); 
    mainMenuInit(windowLayer, window);
    
    layer_add_child(windowLayer, menu_layer_get_layer(getMainMenuLayer()));
}

static void mainWindowUnload(Window *window) {
    
    mainMenuDeinit();
}