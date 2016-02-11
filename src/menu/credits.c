#include <pebble.h>
#include "menu/credits.h"

typedef struct {
    char  *text;
    GFont font;
} text;

typedef struct {
    text title;
    text subtitle;
    text body;
    text footer;
} card;

static Window      *s_creditsWindow;
static Layer       *s_creditsWindowLayer;
static ScrollLayer *s_creditsScrollLayer;

static TextLayer   *s_titleLayer;
static TextLayer   *s_subtitleLayer;
static TextLayer   *s_bodyLayer;
static TextLayer   *s_footerLayer;

static card s_creditsCard;

static void init();
static void load(Window *window);
static void unload(Window *window);

static void createWindowLayer(Window *window);
static void deleteWindowLayer();

static void createScrollLayer(Window *window);
static void deleteScrollLayer();

static void createTextLayers();
static void deleteTextLayers();
static void scrollLayerAddText(TextLayer *textLayer, text textItem);

static GRect getWindowBounds();
static GRect getMaxTextBounds();

void showCredits() {
    
    init();
}

static void init() {
    
    s_creditsWindow = window_create();
    
    window_set_window_handlers(s_creditsWindow, (WindowHandlers){
        .load   = load,
        .unload = unload
    });
    
    window_stack_push(s_creditsWindow, true);
}

static void load(Window *window) {
    
    createWindowLayer(window);
    createScrollLayer(window);
    createTextLayers();    
}

static void unload(Window *window) {
    
    deleteTextLayers();
    deleteScrollLayer();
    deleteWindowLayer();
}

static void createWindowLayer(Window *window) {
    
    s_creditsWindowLayer = window_get_root_layer(window);
}

static void deleteWindowLayer() {
    
    layer_destroy(s_creditsWindowLayer);
    s_creditsScrollLayer = NULL;
}

static void createScrollLayer(Window *window) {
    
    s_creditsScrollLayer = scroll_layer_create(getMaxTextBounds());
    
    scroll_layer_set_click_config_onto_window(s_creditsScrollLayer, window);
}

static void deleteScrollLayer() {
    
    scroll_layer_destroy(s_creditsScrollLayer);
    
    s_creditsScrollLayer = NULL;
}

static void createTextLayers() {
    
    s_creditsCard = (card){
        .title = (text){
            .text = "Credits",
            .font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD)
        },
        .subtitle = (text){
            .text = "Font Awesome Icons",
            .font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)
        },
        .body = (text){
            .text = "Icons made by Dave Gandy are licensed under CC BY 3.0.\n",
            .font = fonts_get_system_font(FONT_KEY_GOTHIC_18)
        },
        .footer = (text){
            .text = "http://fontawesome.io\nhttp://creativecommons.org/licenses/by/3.0",
            .font = fonts_get_system_font(FONT_KEY_GOTHIC_09)
        }
    };
    
    s_titleLayer = text_layer_create(getMaxTextBounds());
    scrollLayerAddText(s_titleLayer, s_creditsCard.title);

    s_subtitleLayer = text_layer_create(getMaxTextBounds());
    scrollLayerAddText(s_subtitleLayer, s_creditsCard.subtitle);

    s_bodyLayer = text_layer_create(getMaxTextBounds());
    scrollLayerAddText(s_bodyLayer, s_creditsCard.body);

    s_footerLayer = text_layer_create(getMaxTextBounds());
    scrollLayerAddText(s_footerLayer, s_creditsCard.footer);
}

static void scrollLayerAddText(TextLayer *textLayer, text textItem) {
            
    text_layer_set_text(textLayer, textItem.text);
    text_layer_set_font(textLayer, textItem.font);
    
    GSize maxSize = text_layer_get_content_size(textLayer);
    
    text_layer_set_size(textLayer, maxSize);
    
    scroll_layer_set_content_size(
        s_creditsScrollLayer,
        GSize(
            getWindowBounds().size.w,
            scroll_layer_get_content_size(s_creditsScrollLayer).h + maxSize.h
        )
    );
    
    scroll_layer_add_child(s_creditsScrollLayer, text_layer_get_layer(textLayer));

    layer_add_child(s_creditsWindowLayer, scroll_layer_get_layer(s_creditsScrollLayer));
}

static void deleteTextLayers() {

    text_layer_destroy(s_titleLayer);
    text_layer_destroy(s_subtitleLayer);
    text_layer_destroy(s_bodyLayer);
    text_layer_destroy(s_footerLayer);
    
    s_titleLayer    = NULL;
    s_subtitleLayer = NULL;
    s_bodyLayer     = NULL;
    s_footerLayer   = NULL;
}

static GRect getWindowBounds() {
    
    return layer_get_frame(s_creditsWindowLayer);
}

static GRect getMaxTextBounds() {
    
    return GRect(0, 0, getWindowBounds().size.w, 2000);
}