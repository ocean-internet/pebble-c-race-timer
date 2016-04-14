#include "notifications/vibrate.h"

void doVibrate(int seconds) {
    
    if(seconds == 0) {
        
        vibes_long_pulse();
        
    } else if(seconds <= 5) {
        
        vibes_short_pulse();
    
    } else if(seconds <= 10) {
        
        vibes_double_pulse();
    }
}
