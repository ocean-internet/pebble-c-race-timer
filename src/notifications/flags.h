#pragma once
#include <pebble.h>
#include "menu/start-signals-menu.c"

void flagsCreate();
void flagsDestroy();

void showClassFlag(Window *window);
void showPapaFlag(Window *window);

void hideClassFlag();
void hidePapaFlag();

void hideFlags();

void doFlags(int minutes, int seconds, Window *window);