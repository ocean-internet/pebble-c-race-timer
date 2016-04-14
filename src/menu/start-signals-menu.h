#pragma once
#include <pebble.h>

#define START_SIGNALS_KEY 10

typedef struct Signal {
    bool signal;    
    char *text;
} __attribute__((__packed__)) Signal;

Signal StartSignals[10];

void showStartSignalsMenu();
