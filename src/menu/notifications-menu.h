#pragma once
#include <pebble.h>

#define  NOTIFICATIONS_KEY 11

typedef struct Notification {
    bool notification;    
    char *label;
} __attribute__((__packed__)) Notification;

Notification Notifications[3];

void showNotificationsMenu();