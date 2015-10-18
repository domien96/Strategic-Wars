#ifndef H_GUI
#define H_GUI

#include "common.h"
#include "level.h"
#include "event.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define FILES_ASSETS_PATH "../assets/"
#define FILES_IMAGES_SUBDIRNAME "images"
#define FILES_FONTS_SUBDIRNAME "fonts"
#define FILES_LEVELS_SUBDIRNAME "levels"

void gui_initialize(const char* theme);
void gui_free(void);

void gui_set_level(Level* level);

void gui_set_level_highlight(int row, int col);
void gui_set_build_highlight(CellType cell_type, Owner owner);

void gui_add_message(char* message, ...);
void gui_clear_messages();

void gui_show_path(Path* path);

void gui_get_next_game_event(Event *ev);

void gui_draw_frame(void);

char* gui_show_save_file_dialog();
char* gui_show_load_file_dialog();

void gui_load_theme(const char*);

#endif
