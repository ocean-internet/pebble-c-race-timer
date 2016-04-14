#include "notifications/light.h"

void doLight(int seconds) {
    
    if(seconds == 0) {
        
        light_enable(false);
        light_enable_interaction();
        
    } else if(seconds <= 10) {
        
        light_enable(true);   
    }
}