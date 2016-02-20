#pragma once
#include <pebble.h>

#define START_SIGNALS_KEY 10

typedef struct Signal {
    bool signal;    
    char *text;
} Signal;

Signal StartSignals[11];

void showStartSignalsMenu();
