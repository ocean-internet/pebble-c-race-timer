#include <pebble.h>
#include "menu/basic-menu-layer.h"

static uint16_t getNumSections(MenuLayer *menuLayer, void *data);
static uint16_t getNumRows(   MenuLayer *menuLayer, uint16_t sectionIndex, void *data);

static int16_t  getHeaderHeight(   MenuLayer *menuLayer, uint16_t sectionIndex, void *data);
static int16_t  getSeparatorHeight(MenuLayer *menuLayer, MenuIndex *cellIndex,  void *data);

static void     drawHeader(GContext* context, const Layer *cellLayer, uint16_t sectionIndex, void *data);
static void     drawRow(   GContext* context, const Layer *cellLayer, MenuIndex *cellIndex,  void *data);

static void     selectClick(MenuLayer *menuLayer, MenuIndex *cellIndex, void *data);

BasicMenuLayer *basicMenuLayerCreate(GRect rect, BasicMenuModel* model) {

    BasicMenuLayer *menuLayer = (BasicMenuLayer*) menu_layer_create(rect);
    
    menu_layer_set_callbacks(menuLayer, model, (MenuLayerCallbacks){
        .get_num_sections     = getNumSections,
        .get_num_rows         = getNumRows,
        .get_header_height    = getHeaderHeight,
        .get_separator_height = getSeparatorHeight,
        .draw_header          = drawHeader,
        .draw_row             = drawRow,
        .select_click         = selectClick,
    });

  return menuLayer;
}

void basicMenuLayerDestroy(BasicMenuLayer *menuLayer) {
    
    menu_layer_destroy((MenuLayer*) menuLayer);
}

BasicMenuModel *basicMenuModelCreate(char *title) {
    
    BasicMenuModel *model = malloc(sizeof *model);
    model->title     = title;
    model->itemCount = 0;
    
    return model;
}

void basicMenuModelDestroy(BasicMenuModel *model) {
  
    for (int i=0; i<model->itemCount; i++) {
        free(model->items[i]);
    }
    free(model);
}

void basicMenuModelAddItem(
    BasicMenuModel *model,
    char           *title,
    char           *subtitle,
    BasicMenuIconCallback   *icon,
    BasicMenuSelectCallback *callback
) {
    
    BasicMenuItem *item = malloc(sizeof *item); 
    
    item->title    = title;
    item->subtitle = subtitle;
    item->icon     = icon;
    item->callback = callback;
    
    model->items[model->itemCount++] = item;
}

static uint16_t getNumSections(MenuLayer *menuLayer, void *data) {
    
    return 1;
}

static uint16_t getNumRows(MenuLayer *menuLayer, uint16_t sectionIndex, void *data) {
    
    BasicMenuModel *model = (BasicMenuModel*) data;
    
    if(0 == sectionIndex) {
        
        return model->itemCount;
        
    } else {
        
        return 0;
    }
}

static int16_t getHeaderHeight(MenuLayer *menuLayer, uint16_t sectionIndex, void *data) {
  
    BasicMenuModel *model = (BasicMenuModel*) data;
    
    if(0 == sectionIndex && model->title) {
    
        return MENU_CELL_BASIC_HEADER_HEIGHT+1;
    
    } else {
    
        return 0;
    }
}

static int16_t getSeparatorHeight(MenuLayer *menuLayer, MenuIndex *cellIndex, void *data) {
    
    return 1;
}

static void drawHeader(GContext* context, const Layer *cellLayer, uint16_t sectionIndex, void *data) {
    
    BasicMenuModel *model = (BasicMenuModel*) data;
    
    if(0 == sectionIndex && model->title) {
        
        menu_cell_basic_header_draw(context, cellLayer, model->title);
    }
}

static void drawRow(GContext* context, const Layer *cellLayer, MenuIndex *cellIndex, void *data) {

    BasicMenuModel *model       = (BasicMenuModel*) data;
    BasicMenuItem  *currentItem = model->items[cellIndex->row];
            
    menu_cell_basic_draw(context, cellLayer, currentItem->title, currentItem->subtitle, currentItem->icon(cellIndex->row));
}

static void selectClick(MenuLayer *menuLayer, MenuIndex *cellIndex, void *data) {
  
    BasicMenuModel *model        = (BasicMenuModel*) data;
    BasicMenuItem  *currentItem  = model->items[cellIndex->row];
    
    currentItem->callback(menuLayer, cellIndex, currentItem);
}