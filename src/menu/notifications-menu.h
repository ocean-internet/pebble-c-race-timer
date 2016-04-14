#pragma once
#include <pebble.h>
#include "main.h"

typedef struct Notification {
    bool notification;    
    char *label;
} __attribute__((__packed__)) Notification;

Notification Notifications[3];

void showNotificationsMenu();