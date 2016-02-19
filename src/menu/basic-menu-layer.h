#pragma once
#include <pebble.h>

struct BasicMenuItem;
struct BasicMenuModel;

typedef MenuLayer BasicMenuLayer;

typedef void (*BasicMenuSelectCallback)();

typedef struct BasicMenuItem {
    char    *title;
    char    *subtitle;
    GBitmap *icon;
    BasicMenuSelectCallback callback;
} BasicMenuItem;

typedef struct BasicMenuModel {
    char          *title;
    int            itemCount;
    BasicMenuItem *items[32];
} BasicMenuModel;

BasicMenuLayer *basicMenuLayerCreate(GRect rect, BasicMenuModel *model);
void            basicMenuLayerDestroy(BasicMenuLayer *menuLayer);

BasicMenuModel *basicMenuModelCreate(char *title);
void            basicMenuModelDestroy(BasicMenuModel *model);

void basicMenuModelAddItem(
    BasicMenuModel *model,
    char *title,
    char *subtitle,
    GBitmap *icon,
    BasicMenuSelectCallback callback
);