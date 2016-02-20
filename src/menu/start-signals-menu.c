#include <pebble.h>
#include "menu/basic-menu-layer.h"
#include "menu/start-signals-menu.h"

#define START_SIGNALS_KEY 10

static Window          *s_menuWindow;
static BasicMenuLayer  *s_menuLayer;
static BasicMenuModel  *s_menuModel;
static GBitmap         *s_icons[2];

static void load(  Window *window);
static void unload(Window *window);

static void layerCreate(Window *menuWindow);
static void layerDestroy();

static void menuCreate();
static void menuDestroy();

static void iconsCreate();
static void iconsDestroy();

static GBitmap *getIcon(int minutes);

static void  toggleStartSignal();

void showStartSignalsMenu() {
    
    s_menuWindow = window_create();
        
    window_set_window_handlers(s_menuWindow, (WindowHandlers) {
        .load   = load,
        .unload = unload
    });
    
    window_stack_push(s_menuWindow, true);
}

void hideStartSignalsMenu() {
    
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
    
    s_menuModel = basicMenuModelCreate("Start Signals");
    
    persist_read_data(START_SIGNALS_KEY, &StartSignals, sizeof(StartSignals));
    
    int numberOfSignals = sizeof(StartSignals)/sizeof(StartSignals[0]);
    
    for(int i=0; i<numberOfSignals; i++) {
        
        basicMenuModelAddItem(
            s_menuModel,
            StartSignals[i].text,
            NULL,
            *getIcon,
            toggleStartSignal
        );
    }
}

static void iconsCreate() {
    
    s_icons[false] = gbitmap_create_with_resource(RESOURCE_ID_ICON_SQUARE);
    s_icons[true]  = gbitmap_create_with_resource(RESOURCE_ID_ICON_CHECK);
}

static void iconsDestroy() {
    
    gbitmap_destroy(s_icons[false]);
    gbitmap_destroy(s_icons[true]);
}

static void menuDestroy() {
    
    persist_write_data(START_SIGNALS_KEY, &StartSignals, sizeof(StartSignals));
    
    basicMenuModelDestroy(s_menuModel);
    s_menuModel = NULL;
}

static GBitmap *getIcon(int minutes) {
    
    return s_icons[StartSignals[minutes].signal];
}

static void toggleStartSignal(MenuLayer *menuLayer, MenuIndex *cellIndex, void *data) {
    
    StartSignals[cellIndex->row].signal = !(StartSignals[cellIndex->row].signal);
    menu_layer_reload_data(menuLayer);
}