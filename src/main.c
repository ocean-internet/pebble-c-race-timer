#include "main.h"
#include "menu/main-menu.h"
#include "menu/notifications-menu.h"

void initStartSignals();
void initNotifications();

int main(void) {
    
    initStartSignals();
    initNotifications();
    
    showMainMenu();
    app_event_loop();
}
