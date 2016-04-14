#include <pebble.h>
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

void initStartSignals() {
    
    if (!persist_exists(START_SIGNALS_KEY)) {
        
        StartSignals[0]  = (Signal){ .signal = true,  .text = "   Start"   };
        StartSignals[1]  = (Signal){ .signal = true,  .text = " 1 Minute"  };
        StartSignals[2]  = (Signal){ .signal = false, .text = " 2 Minutes" };
        StartSignals[3]  = (Signal){ .signal = false, .text = " 3 Minutes" };
        StartSignals[4]  = (Signal){ .signal = true,  .text = " 4 Minutes" };
        StartSignals[5]  = (Signal){ .signal = true,  .text = " 5 Minutes" };
        StartSignals[6]  = (Signal){ .signal = false, .text = " 6 Minutes" };
        StartSignals[7]  = (Signal){ .signal = false, .text = " 7 Minutes" };
        StartSignals[8]  = (Signal){ .signal = false, .text = " 8 Minutes" };
        StartSignals[9]  = (Signal){ .signal = false, .text = " 9 Minutes" };
        
        persist_write_data(START_SIGNALS_KEY, &StartSignals, sizeof(StartSignals));
    }
}

void initNotifications() {
    
    if (!persist_exists(NOTIFICATIONS_KEY)) {
        
        Notifications[0]  = (Notification){ .notification = true, .label = " Vibrations"  };
        Notifications[1]  = (Notification){ .notification = true, .label = " Start Flags" };
        Notifications[2]  = (Notification){ .notification = true, .label = " Light"       };
        
        persist_write_data(NOTIFICATIONS_KEY, &Notifications, sizeof(Notifications));
    }
}