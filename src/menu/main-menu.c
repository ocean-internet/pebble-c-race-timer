#include <pebble.h>
#include <stdio.h>
#include "menu/main-menu.h"

static MenuLayer *s_menuLayer;

typedef struct menuItem {
    char    *title;
    char    *subtitle;
    GBitmap *icon;
} menuItem;

static GBitmap  *s_menuIcons[3];
static menuItem *s_mainMenu[4];

static void setIcons();
static void setMenuItems();
static void setMenuLayer(Layer *windowLayer, Window *window);
static void setClickHandler(Window *window);

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static int16_t  menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);

MenuLayer *getMainMenuLayer() {
    
    return s_menuLayer;
}

void mainMenuInit(Layer *windowLayer, Window *window) {
    
    setIcons();
    setMenuItems();
    
    setMenuLayer(windowLayer, window);
    setClickHandler(window);
}

void mainMenuDeinit() {
    
    int numIcons = sizeof(s_menuIcons) / sizeof(s_menuIcons[0]);
    
    // Cleanup the menu icons
    for (int i = 0; i < numIcons; i++) {
        gbitmap_destroy(s_menuIcons[i]);
    }
}

static void setIcons() {
       
    s_menuIcons[0] = gbitmap_create_with_resource(RESOURCE_ID_ICON_CLOCK);
    s_menuIcons[1] = gbitmap_create_with_resource(RESOURCE_ID_ICON_FLAG);
    s_menuIcons[2] = gbitmap_create_with_resource(RESOURCE_ID_ICON_BELL);
}

static void setMenuItems() {
    
    s_mainMenu[0] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[0]->title    = "Race Timer";
    s_mainMenu[0]->subtitle = "Start Race Timer",
    s_mainMenu[0]->icon     = s_menuIcons[0];
    
    s_mainMenu[1] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[1]->title    = "Notificaitions";
    s_mainMenu[1]->subtitle = "Configure start signal times",
    s_mainMenu[1]->icon     = s_menuIcons[1];

    s_mainMenu[2] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[2]->title    = "Start Signals";
    s_mainMenu[2]->subtitle = "Configure countdown notifications",
    s_mainMenu[2]->icon     = s_menuIcons[2];

    s_mainMenu[3] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[3]->title    = "Credits";
    s_mainMenu[3]->subtitle = NULL;
    s_mainMenu[3]->icon     = NULL;
}

static void setMenuLayer(Layer * windowLayer, Window *window) {
    
    GRect bounds       = layer_get_frame(windowLayer);
    
    // Create the menu layer
    s_menuLayer = menu_layer_create(bounds);
    
    menu_layer_set_highlight_colors(s_menuLayer, GColorBabyBlueEyes, GColorBlack);
    
    menu_layer_set_callbacks(s_menuLayer, NULL, (MenuLayerCallbacks){
        .get_num_sections  = menu_get_num_sections_callback,
        .get_num_rows      = menu_get_num_rows_callback,
        .get_header_height = menu_get_header_height_callback,
        .draw_header       = menu_draw_header_callback,
        .draw_row          = menu_draw_row_callback,
        .select_click      = menu_select_callback,
    });
}

static void setClickHandler(Window *window) {
    
    // Bind the menu layer's click config provider to the window for interactivity
    menu_layer_set_click_config_onto_window(s_menuLayer, window);
}
    

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return sizeof(s_mainMenu) / sizeof(s_mainMenu[0]);
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {

    menu_cell_basic_header_draw(ctx, cell_layer, "Race Timer");
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    
    menuItem *currentItem = s_mainMenu[cell_index->row];
        
    menu_cell_basic_draw(ctx, cell_layer, currentItem->title, currentItem->subtitle, currentItem->icon);
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

}