#pragma once

MenuLayer *getMenuLayer();

void setMenuLayer(Window *window);

static void main_menu_init();

static void menu_get_num_sections_callback();
static void menu_get_num_rows_callback();
static void menu_get_header_height_callback();
static void menu_draw_header_callback();
static void menu_draw_row_callback();
static void menu_select_callback();
