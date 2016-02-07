#include <pebble.h>
#include "menu/main-menu.h"

static MenuLayer *s_menuLayer;

typedef struct menuItem {
    char    *title;
    char    *subtitle;
    GBitmap *icon;
} menuItem;

static GBitmap  *s_menuIcons[3];
static menuItem *s_mainMenu[4];

MenuLayer *getMenuLayer() {
    
    return s_menuLayer;
}

void setMenuLayer(Window *window) {
    
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_frame(window_layer);
    
    // Create the menu layer
    s_menuLayer = menu_layer_create(bounds);
    
    menu_layer_set_callbacks(s_menuLayer, NULL, (MenuLayerCallbacks){
        .get_num_sections  = menu_get_num_sections_callback,
        .get_num_rows      = menu_get_num_rows_callback,
        .get_header_height = menu_get_header_height_callback,
        .draw_header       = menu_draw_header_callback,
        .draw_row          = menu_draw_row_callback,
        .select_click      = menu_select_callback,
    });
}

static void main_menu_init() {
    
    s_menuIcons[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_CLOCK_BLACK);
    s_menuIcons[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_FLAG_BLACK);
    s_menuIcons[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BELL_BLACK);
        
    s_mainMenu[0] = &(menuItem){
        .title    = "Race Timer",
        .subtitle = "Start Race Timer",
        .icon     = s_menuIcons[0]
    };
    s_mainMenu[1] = &(menuItem){
        .title    = "Start Signals",
        .subtitle = "Configure start signal times",
        .icon     = s_menuIcons[1]
    };
    s_mainMenu[2] = &(menuItem){
        .title    = "Notificaitions",
        .subtitle = "Configure countdown notifications",
        .icon     = s_menuIcons[2]
    };
    s_mainMenu[3] = &(menuItem){
        .title    = "Credits",
        .subtitle = NULL,
        .icon     = NULL
    };
}                            
                             