#include <pebble.h>
#include "menu/basic-menu-layer.h"
#include "menu/main-menu.h"
#include "race-timer.h"
#include "menu/start-signals-menu.h"
#include "menu/notifications-menu.h"
#include "menu/credits.h"

static const int clockIcon = 0;
static const int flagIcon  = 1;
static const int bellIcon  = 2;

static Window          *s_menuWindow;
static BasicMenuLayer  *s_menuLayer;
static BasicMenuModel  *s_menuModel;
static GBitmap         *s_icons[4];

static void load(  Window *window);
static void unload(Window *window);

static void layerCreate(Window *menuWindow);
static void layerDestroy();

static void menuCreate();
static void menuDestroy();

static void iconsCreate();
static void iconsDestroy();

static void selectRaceTimer();
static void selectStartSignals();
static void selectNotifications();
static void selectCredits();

GBitmap *getIcon(int i);

void showMainMenu() {
    
    s_menuWindow = window_create();
        
    window_set_window_handlers(s_menuWindow, (WindowHandlers) {
        .load   = load,
        .unload = unload
    });
    
    window_stack_push(s_menuWindow, true);
}

void hideMainMenu() {
    
    window_stack_remove(s_menuWindow, true);
}

static void load(Window *window) {
    
    iconsCreate();
    menuCreate();
    layerCreate(window);
}

static void unload(Window *window) {
    
    layerDestroy();
    menuDestroy();
    iconsDestroy();
    window_destroy(window);
    window = NULL;
}

static void layerCreate(Window *menuWindow) {

    Layer *rootWindowLayer = window_get_root_layer(menuWindow);
    GRect bounds           = layer_get_frame(rootWindowLayer);
    
    s_menuLayer = basicMenuLayerCreate(bounds, s_menuModel);
        
    menu_layer_set_highlight_colors(s_menuLayer, GColorBabyBlueEyes, GColorBlack);
    
    menu_layer_set_click_config_onto_window(s_menuLayer, menuWindow);
    
    layer_add_child(rootWindowLayer, menu_layer_get_layer(s_menuLayer));
}

static void layerDestroy() {
 
    basicMenuLayerDestroy(s_menuLayer);
    s_menuLayer = NULL;
}

static void menuCreate() {
    
    s_menuModel = basicMenuModelCreate("Race Timer");
    
    basicMenuModelAddItem(
        s_menuModel,
        "Race Timer",
        "Start Race Timer",
        *getIcon,
        selectRaceTimer
    );
    
    basicMenuModelAddItem(
        s_menuModel,
        "Start Signals",
        "Configure start signals",
        *getIcon,
        selectStartSignals
    );
    
    basicMenuModelAddItem(
        s_menuModel,
        "Notificaitions",
        "Configure notifications",
        *getIcon,
        selectNotifications
    );
    
    basicMenuModelAddItem(
        s_menuModel,
        "Credits",
        NULL,
        *getIcon,
        selectCredits
    );
}

static void menuDestroy() {
    
    basicMenuModelDestroy(s_menuModel);
    s_menuModel = NULL;
}

static void iconsCreate() {
    
    s_icons[clockIcon] = gbitmap_create_with_resource(RESOURCE_ID_ICON_CLOCK);
    s_icons[flagIcon]  = gbitmap_create_with_resource(RESOURCE_ID_ICON_FLAG);
    s_icons[bellIcon]  = gbitmap_create_with_resource(RESOURCE_ID_ICON_BELL);
}

static void iconsDestroy() {
    
    gbitmap_destroy(s_icons[clockIcon]);
    gbitmap_destroy(s_icons[flagIcon]);
    gbitmap_destroy(s_icons[bellIcon]);
}

GBitmap *getIcon(int i) {
    
    return s_icons[i];
}

static void selectRaceTimer() {
    
    showRaceTimer();
}
static void selectStartSignals() {
    
    showStartSignalsMenu();
}
static void selectNotifications() {
    
    showNotificationsMenu();
}
static void selectCredits() {

    showCredits();
}
