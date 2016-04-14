#include "race-timer.h"

static Window    *s_raceTimerWindow;
static TextLayer *s_timeLayer;
static TextLayer *s_countdownLayer;

static void load(  Window *window);
static void unload(Window *window);

static void timeLayerCreate();
static void timeLayerDestroy();

static void countdownLayerCreate();
static void countdownLayerDestroy();

static void clickConfigProvider(Window *window);

static void bumpTimer();
static void resetTimer();

static void tick();
static void tock();

static void resetCountdown();
static void drawCountdown();
static void drawTime();

static int  getMaxTime();

static void setStyle();

bool timerRunning = false;

uint32_t millis  = 0;
int      maxTime = 0;
int      minutes = 0;
int      seconds = 0;

static char timeString[]      = "00:00:00";
static char countdownString[] = "0:00";

static GColor8  timeBackground;
static GColor8  timeText;
static uint32_t timeFont;
static int      timeHeight;

static GColor8  countdownBackground;
static GColor8  countdownText;
static uint32_t countdownFont;
static int      countdownHeight;

static AppTimer *milliTimer = NULL;

static void setStyle() {
    
    timeBackground = GColorBlack;
    timeText       = GColorBabyBlueEyes;
    timeFont       = RESOURCE_ID_FONT_LEKTON_BOLD_24;
    timeHeight     = 33;
    
    countdownBackground = GColorClear;
    countdownText       = GColorBlack;
    countdownFont       = RESOURCE_ID_FONT_TYPEWRITER_78;
    countdownHeight     = 78;
}

void showRaceTimer() {
        
    s_raceTimerWindow = window_create();
        
    window_set_window_handlers(s_raceTimerWindow, (WindowHandlers) {
        .load   = load,
        .unload = unload
    });

    window_stack_push(s_raceTimerWindow, true);
}

static void clickConfigProvider(Window *window) {

    window_single_click_subscribe(BUTTON_ID_SELECT, bumpTimer);
    window_long_click_subscribe(  BUTTON_ID_SELECT, 0, resetTimer, NULL);
}

static void bumpTimer() {
    
    millis  = time_ms(NULL, NULL);
    
    if(!timerRunning) {
        
        resetCountdown();
        timerRunning = true;
        
        showClassFlag(s_raceTimerWindow);
        
    } else if(seconds > 30) {
        
        minutes++;
    }
    
    seconds = 0;
    drawCountdown();
    
    if(milliTimer) {
        
        app_timer_cancel(milliTimer);
        milliTimer = NULL;
    }
}

static void resetTimer() {
    
    if(milliTimer) {
        
        app_timer_cancel(milliTimer);
        milliTimer = NULL;
    }
    
    timerRunning = false;
    resetCountdown();
}

static void load(Window *window) {
    
    persist_read_data(NOTIFICATIONS_KEY, &Notifications, sizeof(Notifications));
        
    window_set_click_config_provider(s_raceTimerWindow, (ClickConfigProvider) clickConfigProvider);
    
    setStyle();
        
    timeLayerCreate();
    countdownLayerCreate();
    flagsCreate();
    
    resetCountdown();
    
    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler)tick);
    
    drawTime();
}

static void unload(Window *window) {
    
    tick_timer_service_unsubscribe();
    
    flagsDestroy();
    countdownLayerDestroy();
    timeLayerDestroy();
    
    window_destroy(window);
}

static void timeLayerCreate() {
                    
    GRect bounds = layer_get_bounds(window_get_root_layer(s_raceTimerWindow));

    s_timeLayer = text_layer_create(
        GRect(0,0, bounds.size.w, timeHeight)
    );

    text_layer_set_background_color(s_timeLayer, timeBackground);    
    text_layer_set_text_color(      s_timeLayer, timeText);    
    text_layer_set_font(            s_timeLayer, fonts_load_custom_font(resource_get_handle(timeFont)));    
    text_layer_set_text_alignment(  s_timeLayer, GTextAlignmentCenter);
    
    layer_add_child(window_get_root_layer(s_raceTimerWindow), text_layer_get_layer(s_timeLayer));
}

static void countdownLayerCreate() {
                    
    GRect bounds = layer_get_bounds(window_get_root_layer(s_raceTimerWindow));

    s_countdownLayer = text_layer_create(
        GRect(-16, 25, bounds.size.w+20, countdownHeight)
    );

    text_layer_set_background_color(s_countdownLayer, countdownBackground);
    text_layer_set_text_color(      s_countdownLayer, countdownText);
    text_layer_set_font(            s_countdownLayer, fonts_load_custom_font(resource_get_handle(countdownFont)));
    text_layer_set_text_alignment(  s_countdownLayer, GTextAlignmentCenter);

    layer_add_child(window_get_root_layer(s_raceTimerWindow), text_layer_get_layer(s_countdownLayer));
}

static void countdownLayerDestroy() {
    
    text_layer_destroy(s_countdownLayer);
}

static void timeLayerDestroy() {
    
    text_layer_destroy(s_timeLayer);
}

static void resetCountdown() {
            
    minutes = getMaxTime();
    seconds = 0;
    
    drawCountdown();
    
    hideFlags();
}

static void drawCountdown() {
        
    snprintf(countdownString, sizeof countdownString, "%01d:%02d", minutes, seconds);
    text_layer_set_text(s_countdownLayer, countdownString);
    
    if(StartSignals[minutes].signal) {
        
        if(Notifications[VIBRATE].notification) doVibrate(seconds);
        if(Notifications[FLAGS].notification)   doFlags(minutes, seconds, s_raceTimerWindow);
        if(Notifications[LIGHT].notification)   doLight(seconds);
    }
}

static void drawTime() {
    
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    strftime(
        timeString,
        sizeof timeString,
        clock_is_24h_style() ? "%H:%M:%S" : "%I:%M:%S",
        tick_time
    );

    text_layer_set_text(s_timeLayer, timeString);
}

int getMaxTime() {
    
    int maxTime = 0;
    
    persist_read_data(START_SIGNALS_KEY, &StartSignals, sizeof(StartSignals));
    
    int numberOfSignals = sizeof(StartSignals)/sizeof(StartSignals[0]);
    
    for(int i=0; i<numberOfSignals; i++) {
        
        if(StartSignals[i].signal) {
            
            maxTime = i;
        }
    }
    
    return maxTime;
}

static void tick() {
    
    if(timerRunning && !milliTimer) {
        
        milliTimer = app_timer_register(millis, (AppTimerCallback)tock, NULL);          
    }
    
    drawTime();
}

static void tock() {
    
    milliTimer = NULL;
    
    switch(seconds) {
            
        case 0:
            seconds = 59;
            minutes--;
            break;
        default:
            seconds--;
            break;
    }
        
    if(minutes < 1 && seconds < 1) {
        
        minutes = 0;
        seconds = 0;
        
        timerRunning = false;
    }
    
    drawCountdown();
}