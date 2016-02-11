#include <pebble.h>
#include "menu/main-menu.h"
#include "menu/credits.h"

typedef struct menuItem {
    char    *title;
    char    *subtitle;
    GBitmap *icon;
    void    (*select)();
} menuItem;

static Window    *s_menuWindow;
static MenuLayer *s_menuLayer;

static GBitmap  *s_menuIcons[3];
static menuItem *s_mainMenu[4];

static void init();
static void load(  Window *window);
static void unload(Window *window);

/*
 * Create/Delete assets
 */
static void createIcons();
static void deleteIcons();

static void createMenuItems();
static void deleteMenuItems();

static void createMenuLayer(Window *window);
static void deleteMenuLayer();

/*
 * MenuLayer Callbacks
 */
static uint16_t getNumSections( MenuLayer *menuLayer, void *data);
static uint16_t getNumRows(     MenuLayer *menuLayer, uint16_t sectionIndex, void *data);
static int16_t  getHeaderHeight(MenuLayer *menuLayer, uint16_t sectionIndex, void *data);

static void drawHeader(GContext* ctx, const Layer *cellLayer, uint16_t  sectionIndex, void *data);
static void drawRow(   GContext* ctx, const Layer *cellLayer, MenuIndex *cellIndex,   void *data);
static void select(                   MenuLayer   *menuLayer, MenuIndex *cellIndex,   void *data);

/*
 * menuItem selected callbacks
 */
static void selectRaceTimer();
static void selectStartSignals();
static void selectNotifications();
static void selectCredits();


void showMainMenu() {
    
    init();
}

static void init() {
    
    s_menuWindow = window_create(); 
    
    window_set_window_handlers(s_menuWindow, (WindowHandlers) {
        .load   = load,
        .unload = unload
    });
    
    window_stack_push(s_menuWindow, true);
}

static void load(Window *window) {
    
    createIcons();
    createMenuItems();
    createMenuLayer(window);
}

static void unload(Window *window) {
    
    deleteMenuLayer(window);
    deleteMenuItems();
    deleteIcons();
    
    window_destroy(window);
    s_menuWindow = NULL;
}

static void createIcons() {
       
    s_menuIcons[0] = gbitmap_create_with_resource(RESOURCE_ID_ICON_CLOCK);
    s_menuIcons[1] = gbitmap_create_with_resource(RESOURCE_ID_ICON_FLAG);
    s_menuIcons[2] = gbitmap_create_with_resource(RESOURCE_ID_ICON_BELL);
}

static void deleteIcons() {
    
    int numIcons = sizeof(s_menuIcons) / sizeof(s_menuIcons[0]);
    
    // Cleanup the menu icons
    for (int i = 0; i < numIcons; i++) {
        gbitmap_destroy(s_menuIcons[i]);
    }
}

static void createMenuItems() {
    
    s_mainMenu[0] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[0]->title    = "Race Timer";
    s_mainMenu[0]->subtitle = "Start Race Timer",
    s_mainMenu[0]->icon     = s_menuIcons[0];
    s_mainMenu[0]->select   = &selectRaceTimer;
    
    s_mainMenu[1] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[1]->title    = "Start Signals";
    s_mainMenu[1]->subtitle = "Configure start signals",
    s_mainMenu[1]->icon     = s_menuIcons[1];
    s_mainMenu[1]->select   = &selectStartSignals;

    s_mainMenu[2] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[2]->title    = "Notificaitions";
    s_mainMenu[2]->subtitle = "Configure notifications",
    s_mainMenu[2]->icon     = s_menuIcons[2];
    s_mainMenu[2]->select   = &selectNotifications;

    s_mainMenu[3] = (menuItem *)malloc(sizeof(menuItem));
    s_mainMenu[3]->title    = "Credits";
    s_mainMenu[3]->subtitle = NULL;
    s_mainMenu[3]->icon     = NULL;
    s_mainMenu[3]->select   = &selectCredits;
}

static void deleteMenuItems() {
    
    int numItems = sizeof(s_mainMenu)  / sizeof(s_mainMenu[0]);
    
    // Cleanup the menu items
    for (int i = 0; i < numItems; i++) {
        free(s_mainMenu[i]);
        s_mainMenu[i] = NULL;
    }
}

static void createMenuLayer(Window *window) {
    
    Layer *windowLayer = window_get_root_layer(window);
        
    GRect bounds = layer_get_frame(windowLayer);
    s_menuLayer  = menu_layer_create(bounds);
    
    menu_layer_set_highlight_colors(s_menuLayer, GColorBabyBlueEyes, GColorBlack);
    
    menu_layer_set_click_config_onto_window(s_menuLayer, window);
    
    menu_layer_set_callbacks(s_menuLayer, NULL, (MenuLayerCallbacks){
        .get_num_sections  = getNumSections,
        .get_num_rows      = getNumRows,
        .get_header_height = getHeaderHeight,
        .draw_header       = drawHeader,
        .draw_row          = drawRow,
        .select_click      = select,
    });
    
    layer_add_child(windowLayer, menu_layer_get_layer(s_menuLayer));
}

static void deleteMenuLayer() {
    
    menu_layer_destroy(s_menuLayer);
    s_menuLayer = NULL;
}
    
static uint16_t getNumSections(MenuLayer *menuLayer, void *data) {

    return 1;
}

static uint16_t getNumRows(MenuLayer *menuLayer, uint16_t sectionIndex, void *data) {

    return sizeof(s_mainMenu) / sizeof(s_mainMenu[0]);
}

static int16_t getHeaderHeight(MenuLayer *menuLayer, uint16_t sectionIndex, void *data) {
    
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void drawHeader(GContext* ctx, const Layer *cellLayer, uint16_t sectionIndex, void *data) {

    menu_cell_basic_header_draw(ctx, cellLayer, "Race Timer");
}

static void drawRow(GContext* ctx, const Layer *cellLayer, MenuIndex *cellIndex, void *data) {
    
    menuItem *currentItem = s_mainMenu[cellIndex->row];
        
    menu_cell_basic_draw(ctx, cellLayer, currentItem->title, currentItem->subtitle, currentItem->icon);
}

static void select(MenuLayer *menuLayer, MenuIndex *cellIndex, void *data) {

    menuItem *currentItem = s_mainMenu[cellIndex->row];
    
    currentItem->select();
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