#pragma once
#include <pebble.h>
#include "main.h"

typedef struct Signal {
    bool signal;    
    char *text;
} __attribute__((__packed__)) Signal;

Signal StartSignals[10];

void initStartSignals();

void showStartSignalsMenu();
