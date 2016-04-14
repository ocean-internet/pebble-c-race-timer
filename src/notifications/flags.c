#include "notifications/flags.h"

static BitmapLayer *s_flagPapaLayer;
static BitmapLayer *s_pennantOneLayer;

static const int papa = 0;
static const int one  = 1;

static GBitmap *s_flags[2];

static int papaFlagDown = 0;
static int papaFlagUp   = 0;

void flagsCreate() {
    
    s_flags[papa] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FLAG_PAPA);
    s_flags[one]  = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PENNANT_ONE);
    
    s_flagPapaLayer   = bitmap_layer_create(GRect(32+48, 168-48, 48, 48));
    s_pennantOneLayer = bitmap_layer_create(GRect(16, 168-48, 48, 48));
    
    bitmap_layer_set_bitmap(s_flagPapaLayer,   s_flags[papa]);
    bitmap_layer_set_bitmap(s_pennantOneLayer, s_flags[one]);
    
    for(int i=1; i < 10; i++) {
        
        if(StartSignals[i].signal && papaFlagDown == 0) {
            
            papaFlagDown = i;
        }
        
        if(StartSignals[i].signal && papaFlagUp < papaFlagDown) {
            
            papaFlagUp = i;
        }
    }
}

static void flagsDestroy() {
    
    gbitmap_destroy(s_flags[papa]);
    gbitmap_destroy(s_flags[one]);
    
    bitmap_layer_destroy(s_flagPapaLayer);
    bitmap_layer_destroy(s_pennantOneLayer);
}

void showClassFlag(Window *window) {
     
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_pennantOneLayer));
}

void showPapaFlag(Window *window) {
    
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_flagPapaLayer));
}

void hideClassFlag() {
    
    layer_remove_from_parent(bitmap_layer_get_layer(s_pennantOneLayer));
}

void hidePapaFlag() {
    
    layer_remove_from_parent(bitmap_layer_get_layer(s_flagPapaLayer));
}

void hideFlags() {
    
    hideClassFlag();
    hidePapaFlag();
}

void doFlags(int minutes, int seconds, Window *window) {
    
    if(seconds == 0) {
        
        if(minutes == 0) {
            
            hideClassFlag();
            hidePapaFlag();
        
        } else if(minutes == papaFlagDown) {
            
            hidePapaFlag();
            
        } else if(minutes == papaFlagUp) {
            
            showPapaFlag(window);
        
        }
    }
}