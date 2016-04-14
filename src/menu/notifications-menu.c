#include <pebble.h>
#include "menu/basic-menu-layer.h"
#include "menu/notifications-menu.h"

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

static GBitmap *getIcon(int index);

static void  toggleNotification();

void showNotificationsMenu() {
    
    s_menuWindow = window_create();
        
    window_set_window_handlers(s_menuWindow, (WindowHandlers) {
        .load   = load,
        .unload = unload
    });
    
    window_stack_push(s_menuWindow, true);
}

void hideNotificationsMenu() {
    
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
    
    s_menuModel = basicMenuModelCreate("Notifications");
    
    persist_read_data(NOTIFICATIONS_KEY, &Notifications, sizeof(Notifications));
    
    int numberOfNotifications = sizeof(Notifications)/sizeof(Notifications[0]);
    
    for(int i=0; i<numberOfNotifications; i++) {
        
        basicMenuModelAddItem(
            s_menuModel,
            Notifications[i].label,
            NULL,
            *getIcon,
            toggleNotification
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
    
    persist_write_data(NOTIFICATIONS_KEY, &Notifications, sizeof(Notifications));
    
    basicMenuModelDestroy(s_menuModel);
    s_menuModel = NULL;
}

static GBitmap *getIcon(int index) {
    
    return s_icons[Notifications[index].notification];
}

static void toggleNotification(MenuLayer *menuLayer, MenuIndex *cellIndex, void *data) {
    
    Notifications[cellIndex->row].notification = !(Notifications[cellIndex->row].notification);
    menu_layer_reload_data(menuLayer);
}