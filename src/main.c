#include <pebble.h>
#include "menu/main-menu.h"

int main(void) {
    
    showMainMenu();
    app_event_loop();
}