#include <pebble.h>
#include "menu/basic-menu-layer.h"
#include "menu/main-menu.h"
#include "menu/credits.h"

static Window          *s_menuWindow;
static BasicMenuLayer  *s_menuLayer;
static BasicMenuModel  *s_menuModel;

static void load(  Window *window);
static void unload(Window *window);

static void layerCreate(Window *menuWindow);
static void layerDestroy();

static void menuCreate();
static void menuDestroy();

static void selectRaceTimer();
static void selectStartSignals();
static void selectNotifications();
static void selectCredits();

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
    
    menuCreate();
    layerCreate(window);
}

static void unload(Window *window) {
    
    layerDestroy();
    menuDestroy();
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
        gbitmap_create_with_resource(RESOURCE_ID_ICON_CLOCK),
        selectRaceTimer
    );
    
    basicMenuModelAddItem(
        s_menuModel,
        "Start Signals",
        "Configure start signals",
        gbitmap_create_with_resource(RESOURCE_ID_ICON_FLAG),
        selectStartSignals
    );
    
    basicMenuModelAddItem(
        s_menuModel,
        "Notificaitions",
        "Configure notifications",
        gbitmap_create_with_resource(RESOURCE_ID_ICON_BELL),
        selectNotifications
    );
    
    basicMenuModelAddItem(
        s_menuModel,
        "Credits",
        NULL,
        NULL,
        selectCredits
    );
}

static void menuDestroy() {
    
    basicMenuModelDestroy(s_menuModel);
    s_menuModel = NULL;
}

static void selectRaceTimer() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Select Race Timer");
}
static void selectStartSignals() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Select Start Signals");
}
static void selectNotifications() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Select Notifications");
}
static void selectCredits() {

    showCredits();
}