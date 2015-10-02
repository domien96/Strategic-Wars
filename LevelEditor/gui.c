#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gui.h"
#include "common.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#define UI_FONT_SIZE 18
#define UI_TEXT_GAP_X 4
#define UI_TEXT_GAP_Y 4
#define UI_BUTTON_GAP_X 8
#define UI_BUTTON_GAP_Y 4
#define UI_MESSAGE_GAP_Y 2
#define UI_DEBUG_GAP_X 4
#define UI_DEBUG_GAP_Y 4
#define UI_DEBUG_W 100
#define UI_BASE_MARGIN 20

#define GUI_MAX_FILENAME_LEN 2048
#define GUI_MSG_MAX_LEN 1024
#define GUI_MSG_MAX_COUNT 5

#define GUI_MAX_STEPS 150

#define FILES_MAX_FILENAME_LEN 1024

#define FONT_ARIAL "arial.ttf"
#define FONT_ARIALN "arialn.ttf"
#define FONT_DIGITAL "digital.ttf"
#define FONT_DOTTED "dotted.ttf"
#define FONT_SQUARED "squared.ttf"

#define TICK_PER_S 40



typedef struct {
    int x;
    int y;
    int w;
    int h;
    int is_build_selector;
    ButtonFunction function;
    CellType cell_type;   
    Owner owner;
    char* text;
} GuiButton;

typedef struct 
{
    int ready;

    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP** tiles_by_cell_type;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* select_bitmap;
    ALLEGRO_BITMAP* actor_bitmap;
    ALLEGRO_BITMAP* step_bitmap;
    ALLEGRO_BITMAP* flag_bitmaps[2];

    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;

    bool show_fps;
    int fps;
    int fps_accum;
    double fps_time;

    int level_pixel_w;
    int level_pixel_h;
    int level_cell_w;
    int level_cell_h;

    int window_w;
    int window_h;

    int tile_w;
    int tile_h;

    int level_offset_x;
    int level_padding_x;
    int level_offset_y;
    int level_padding_y;

    Level* level;
    int level_highlight_row;
    int level_highlight_col;
    CellType build_highlight_cell_type;
    Owner build_highlight_owner;

    Event prev_interaction_event;
    int mouse_down;

    int button_count;
    GuiButton* buttons;

    char* messages;
    int next_message_index;

    char dialog_filename[GUI_MAX_FILENAME_LEN];

    Path path;
} Gui;

static Gui* _strategic_wars_gui = NULL;
static Gui* get_gui()
{
    Gui* res = _strategic_wars_gui;
    if(!res->ready) { 
        printf("GUI ERROR: Please call the gui_initialize method first!\n"); 
        exit(-1);
    } 
    return res;
}

void gui_show_path(Path* path)
{
    Gui* gui = get_gui();

    if (path == NULL || path->step_count == 0) {
        gui->path.step_count = 0;
        gui->path.distance = 0;
        return;
    }

    gui->path.step_count = path->step_count;
    gui->path.distance = path->distance;
    if (gui->path.step_count > GUI_MAX_STEPS)
        gui->path.step_count = GUI_MAX_STEPS - 1;

    for (int i = 0; i < gui->path.step_count; i++)
        gui->path.steps[i] = path->steps[i];
}

void gui_clear_messages()
{
    Gui* gui = get_gui();

    for (int i = 0; i < GUI_MSG_MAX_COUNT; i++)
        (gui->messages+(i*GUI_MSG_MAX_LEN))[0] = '\0';
    gui->next_message_index = 0;
}

void gui_add_message(char* message, ...)
{
    Gui* gui = get_gui();
    va_list argptr;
    va_start(argptr, message);

    if (message == NULL) {
        //do nothing
    } else {
        char* msg = gui->messages+(gui->next_message_index*GUI_MSG_MAX_LEN);

        //add \0 at msg end to make sure string is correctly terminated
        msg[GUI_MSG_MAX_LEN] = '\0';

        int message_len = vsnprintf(msg, GUI_MSG_MAX_LEN, message, argptr);

        //stop message at first newline
        for (int i = 0; i < message_len; i++) {
            if (msg[i] == '\r' || msg[i] == '\n')
                msg[i] = '\0';
        }

        gui->next_message_index++;
        if (gui->next_message_index >= GUI_MSG_MAX_COUNT)
            gui->next_message_index = 0;
    }
    va_end(argptr);
}

void gui_set_level(Level* level)
{
    Gui* gui = get_gui();
    gui->level = level;
}

void gui_set_level_highlight(int row, int col)
{
    Gui* gui = get_gui();
    gui->level_highlight_row = row;
    gui->level_highlight_col = col;
}

void gui_set_build_highlight(CellType cell_type, Owner owner)
{
    Gui* gui = get_gui();
    gui->build_highlight_cell_type = cell_type;
    gui->build_highlight_owner = owner;
}

static ALLEGRO_BITMAP* get_tile_by_cell_type(CellType cell_type)
{
    Gui* gui = get_gui();
    return gui->tiles_by_cell_type[(int)cell_type];
}

static void gui_draw_ui()
{
    Gui* gui = get_gui();
    ALLEGRO_BITMAP* ground_bitmap = get_tile_by_cell_type(GROUND);
    for (int i = 0; i < gui->button_count; i++) {
        GuiButton* button = &gui->buttons[i];

        int x = button->x;
        int y = button->y;

        if (button->is_build_selector) {
            al_draw_bitmap(ground_bitmap, x, y, 0);

            if (button->owner != OWNER_NONE)
            {
                ALLEGRO_BITMAP* flag_bitmap = gui->flag_bitmaps[button->owner == OWNER_HUMAN ? 0 : 1];
                al_draw_bitmap(flag_bitmap, x, y, 0);
            }

            ALLEGRO_BITMAP* bitmap = get_tile_by_cell_type(button->cell_type);
            assert(bitmap != NULL);
            al_draw_bitmap(bitmap, x, y, 0);

            if (gui->build_highlight_cell_type == button->cell_type 
                    && gui->build_highlight_owner == button->owner
               ) {
                al_draw_bitmap(gui->select_bitmap, x, y, 0);
            }
        } else {
			al_draw_filled_rounded_rectangle(x, y, x+button->w, y+button->h,
				8, 8, al_map_rgba(0, 0, 0, 255));
            if (button->text != NULL) {
                al_draw_textf(gui->font, al_map_rgb(255, 255, 255),
                        x+(button->w/2), y+((button->h-UI_FONT_SIZE)/2), ALLEGRO_ALIGN_CENTRE, button->text);
            }
        }
    }

    for (int i = 0; i < GUI_MSG_MAX_COUNT; i++) {
        int cur_msg_index = gui->next_message_index - (i+1);
        while (cur_msg_index < 0)
            cur_msg_index += GUI_MSG_MAX_COUNT;
        char* message = &gui->messages[GUI_MSG_MAX_LEN*cur_msg_index];
        if (message[0] != '\0') {
            int w = (2*UI_TEXT_GAP_X) + al_get_text_width(gui->font, message);
            int h = (2*UI_TEXT_GAP_Y) + UI_FONT_SIZE;
            int x = gui->level_offset_x + (2*UI_TEXT_GAP_X);
            int y = gui->level_offset_y + gui->level_pixel_h + (UI_MESSAGE_GAP_Y) + (i * (h + (UI_MESSAGE_GAP_Y)));
            al_draw_filled_rounded_rectangle(x, y, x+w, y+h, 8, 8, al_map_rgba(0, 0, 0, 255));
            al_draw_textf(gui->font, al_map_rgb(255, 255, 255),
                    x+(w/2), y+((h-UI_FONT_SIZE)/2), ALLEGRO_ALIGN_CENTRE, message);
        }
    }

    int w = UI_DEBUG_W;
    int h = UI_FONT_SIZE + (2*UI_TEXT_GAP_Y);
    int x = gui->window_w - (w + UI_DEBUG_GAP_X);
    if (gui->path.step_count > 0) {
        int y = gui->window_h - ((h + UI_DEBUG_GAP_Y) * 3);
        al_draw_filled_rounded_rectangle(x, y, x+w, y+h,
                8, 8, al_map_rgba(0, 0, 0, 200));
        al_draw_textf(gui->font, al_map_rgb(255, 255, 255),
                x+(w/2), y+UI_TEXT_GAP_Y, ALLEGRO_ALIGN_CENTRE, "dist=%d", gui->path.distance);
    }

    if (1) {
        int y = gui->window_h - ((h + UI_DEBUG_GAP_Y) * 2);
        al_draw_filled_rounded_rectangle(x, y, x+w, y+h,
                8, 8, al_map_rgba(0, 0, 0, 200));
        al_draw_textf(gui->font, al_map_rgb(255, 255, 255),
                x+(w/2), y+UI_TEXT_GAP_Y, ALLEGRO_ALIGN_CENTRE, "r=%d c=%d", gui->level_highlight_row, gui->level_highlight_col);
    }
}

static void gui_draw_level()
{
    Gui* gui = get_gui();

    ALLEGRO_BITMAP* ground_bitmap = get_tile_by_cell_type(GROUND);

    if (gui->level != NULL) {
        Level* level = gui->level;
        for (int row = 0; row < level->height; row++) {
            for (int col = 0; col < level->width; col++) {
                Cell* cell = &level->cells[row][col];

                int x = gui->level_offset_x + (col * gui->tile_w);
                int y = gui->level_offset_y + (row * gui->tile_h);

                al_draw_bitmap(ground_bitmap, x, y, 0);

                if (cell->type != GROUND) {
                    if (cell->owner != OWNER_NONE)
                    {
                        ALLEGRO_BITMAP* flag_bitmap = gui->flag_bitmaps[cell->owner == OWNER_HUMAN ? 0 : 1];
                        al_draw_bitmap(flag_bitmap, x, y, 0);
                    }

                    ALLEGRO_BITMAP* bitmap = get_tile_by_cell_type(cell->type);
                    assert(bitmap != NULL);
                    al_draw_bitmap(bitmap, x, y, 0);
                }

                if (row == gui->level_highlight_row && 
                    col == gui->level_highlight_col)
                {
                    al_draw_bitmap(gui->select_bitmap, x, y, 0);
                }
            }
        }
    }

    for (int i = 0; i < gui->path.step_count; i++) {
        Pos step = gui->path.steps[i];
        int x = gui->level_offset_x + (step.col * gui->tile_w);
        int y = gui->level_offset_y + (step.row * gui->tile_h);
        al_draw_bitmap(gui->step_bitmap, x, y, 0);
    }
}


static ALLEGRO_BITMAP* load_bitmap(char* base_name, const char* theme)
{
    ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_IMAGES_SUBDIRNAME);
    al_append_path_component(path, theme);
    al_set_path_filename(path, base_name);
    al_set_path_extension(path, ".png");

    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
    ALLEGRO_BITMAP* res = al_load_bitmap(filename);

    if (res == NULL){
        printf("Failed to load tile for \"%s\" from file \"%s\"\n", base_name, filename);
        al_destroy_path(path);
        exit(-1);
    }
    al_destroy_path(path);

    return res;
}

static void load_cell_type_bitmap(CellType cell_type, char* base_name, const char* theme)
{
    Gui* gui = _strategic_wars_gui; 

    gui->tiles_by_cell_type[(int)cell_type] = load_bitmap(base_name, theme); 
}

static ALLEGRO_FONT* load_font(char* font_file)
{
    ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_FONTS_SUBDIRNAME);
    al_set_path_filename(path, font_file);

    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

    ALLEGRO_FONT* font = al_load_ttf_font(filename, UI_FONT_SIZE, 0);
	if (font == NULL) {
        printf("Failed to load font \"%s\" from file \"%s\".\nLast Allegro error nr `%d'\n", 
                font_file, filename, al_get_errno());
        al_destroy_path(path);
        exit(-1);
	}
    al_destroy_path(path);
    return font;
}

static void gui_button_init_function(GuiButton* button, int x, int y, int w, int h, ButtonFunction ui_function, char* text)
{
    button->x = x;
    button->y = y;
    button->w = w;
    button->h = h;
    button->is_build_selector = 0;
    button->function = ui_function;
    button->text = text;
}

static void gui_button_init_build_select(GuiButton* button, CellType cell_type, Owner owner)
{
    Gui* gui = _strategic_wars_gui;
    int cell_type_offset = (int) cell_type;
    int owner_offset = owner == OWNER_AI ? 1 : 0;
    int x = UI_BASE_MARGIN;
    int y = UI_BASE_MARGIN;
    button->x = x + (owner_offset * gui->tile_w);
    button->y = y + (cell_type_offset * gui->tile_h);
    button->w = gui->tile_w;
    button->h = gui->tile_h;
    button->is_build_selector = 1;
    button->text = NULL;
    button->cell_type = cell_type;
    button->owner = owner;
}

void gui_initialize(const char* theme) 
{
    if (_strategic_wars_gui != NULL)
    {
        fprintf(stderr, "GUI ERROR: GUI was already initialised. Please call gui_free first.");
        exit(-1);
    }
    _strategic_wars_gui = calloc(1, sizeof(Gui));
    Gui* gui = _strategic_wars_gui;

    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
	al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    gui->display = al_create_display(640, 480);
    al_set_window_position(gui->display, 20, 20);
    al_set_window_title(gui->display, "Strategic Wars Level Editor");

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	gui->tiles_by_cell_type = (ALLEGRO_BITMAP**) calloc(CELL_TYPE_COUNT, sizeof(ALLEGRO_BITMAP*));

    load_cell_type_bitmap(GROUND, "terrain", theme);
    load_cell_type_bitmap(WATER, "water", theme);
    load_cell_type_bitmap(ROCK, "rock", theme);
    load_cell_type_bitmap(HEADQUARTER, "hq", theme);
    load_cell_type_bitmap(BRIDGE, "bridge", theme);
    load_cell_type_bitmap(UNIT_1, "infantry", theme);
    load_cell_type_bitmap(UNIT_2, "archery", theme);
    load_cell_type_bitmap(UNIT_3, "fire", theme);
    gui->select_bitmap = load_bitmap("select", theme);
    gui->actor_bitmap = load_bitmap("actor", theme);
    gui->step_bitmap = load_bitmap("path", theme);
    gui->flag_bitmaps[0] = load_bitmap("flag1", theme);
    gui->flag_bitmaps[1] = load_bitmap("flag2", theme);

    al_set_display_icon(gui->display, gui->tiles_by_cell_type[(int)UNIT_3]);

    gui->show_fps = 1;

    gui->tile_w = al_get_bitmap_width(gui->step_bitmap);
    gui->tile_h = al_get_bitmap_height(gui->step_bitmap);
    gui->level_cell_w = LEVEL_MAX_WIDTH;
    gui->level_cell_h = LEVEL_MAX_HEIGHT;
    gui->level_pixel_w = gui->level_cell_w * gui->tile_w;
    gui->level_pixel_h = gui->level_cell_h * gui->tile_h;
    gui->level_offset_x = (2*gui->tile_w) + (2*UI_BASE_MARGIN);
    gui->level_padding_x = 2*UI_BASE_MARGIN;
    gui->level_offset_y = 3*UI_BASE_MARGIN;
    gui->level_padding_y = ((GUI_MSG_MAX_COUNT+1)*UI_MESSAGE_GAP_Y)+(GUI_MSG_MAX_COUNT*(UI_FONT_SIZE+(UI_TEXT_GAP_Y*2)));
    gui->window_w = gui->level_offset_x + gui->level_pixel_w + gui->level_padding_x;
    gui->window_h = gui->level_offset_y + gui->level_pixel_h + gui->level_padding_y;

    al_resize_display(gui->display, gui->window_w, gui->window_h);

    gui->font = load_font(FONT_ARIAL);
    //gui->font = load_font(FONT_ARIALN);
    //gui->font = load_font(FONT_DIGITAL);
    //gui->font = load_font(FONT_DOTTED);
    //gui->font = load_font(FONT_SQUARED);

    int button_count = CELL_TYPE_COUNT + 4 + 3;
    gui->buttons = calloc(button_count, sizeof(GuiButton));
    int cur_button_index = 0;
    for (int i = 0; i < CELL_TYPE_COUNT; i++) {
        CellType cell_type = (CellType) i;
        if (cell_type == UNIT_1 || cell_type == UNIT_2 || cell_type == UNIT_3 || cell_type == HEADQUARTER) {
            gui_button_init_build_select(&gui->buttons[cur_button_index++], cell_type, OWNER_HUMAN);
            gui_button_init_build_select(&gui->buttons[cur_button_index++], cell_type, OWNER_AI);
        } else {
            gui_button_init_build_select(&gui->buttons[cur_button_index++], cell_type, OWNER_NONE);
        }
    }
    int cur_button_x = gui->level_offset_x + UI_BASE_MARGIN;
    int cur_button_y = UI_BASE_MARGIN;
    int button_w = (2*UI_TEXT_GAP_X) + 80;
    int button_h = (2*UI_TEXT_GAP_Y) + UI_FONT_SIZE;
    gui_button_init_function(&gui->buttons[cur_button_index++], cur_button_x, cur_button_y, button_w, button_h, UI_SAVE, "Save...");
    cur_button_x += button_w + (2*UI_BUTTON_GAP_Y);
    gui_button_init_function(&gui->buttons[cur_button_index++], cur_button_x, cur_button_y, button_w, button_h, UI_LOAD, "Load...");
    cur_button_x += button_w + (2*UI_BUTTON_GAP_Y);
    gui_button_init_function(&gui->buttons[cur_button_index++], cur_button_x, cur_button_y, button_w, button_h, UI_CLEAR, "Clear");
    gui->button_count = cur_button_index;
    assert(button_count >= gui->button_count);

    gui->messages = (char*) malloc((GUI_MSG_MAX_COUNT * GUI_MSG_MAX_LEN) + 1);
	memset(gui->messages, 0, (GUI_MSG_MAX_COUNT * GUI_MSG_MAX_LEN) + 1);

    gui->path.step_count = 0;
    gui->path.steps = (Pos*) calloc(GUI_MAX_STEPS, sizeof(Pos));

    gui->timer = al_create_timer(1.0 / TICK_PER_S);
    gui->queue = al_create_event_queue();
    al_register_event_source(gui->queue, al_get_keyboard_event_source());
    al_register_event_source(gui->queue, al_get_mouse_event_source());
    al_register_event_source(gui->queue, al_get_display_event_source(gui->display));
    al_register_event_source(gui->queue, al_get_timer_event_source(gui->timer));

	gui->ready = 1;

    al_start_timer(gui->timer);
}

void gui_free()
{
    Gui* gui = get_gui();
	gui->ready = 0;
	if (gui->tiles_by_cell_type != NULL) 
		for (int i = 0; i < CELL_TYPE_COUNT; i++)
            if (gui->tiles_by_cell_type[i] != NULL) 
                al_destroy_bitmap(gui->tiles_by_cell_type[i]);
	free(gui->tiles_by_cell_type);
	gui->tiles_by_cell_type = NULL;

    free(gui->messages);
	
	al_stop_timer(gui->timer);
	al_unregister_event_source(gui->queue, al_get_keyboard_event_source());
	al_unregister_event_source(gui->queue, al_get_display_event_source(gui->display));
	al_unregister_event_source(gui->queue, al_get_timer_event_source(gui->timer));
	al_destroy_event_queue(gui->queue);
	
	if (gui->font != NULL) al_destroy_font(gui->font);

	al_destroy_display(gui->display);
    gui->display = NULL;

	al_uninstall_mouse();
	al_uninstall_keyboard();
	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();

    al_uninstall_system();

    free(_strategic_wars_gui);
    _strategic_wars_gui = NULL;
}

void gui_draw_frame()
{
    Gui* gui = get_gui();
    double t = al_get_time();

	al_draw_filled_rectangle(
		0, 0,
		gui->window_w, gui->window_h,
		al_map_rgb(207, 207, 207));
	gui_draw_level();
	gui_draw_ui();

    if (gui->font) {
		if (gui->show_fps) {
            int w = UI_DEBUG_W;
            int h = UI_FONT_SIZE + (2*UI_TEXT_GAP_Y);
            int x = gui->window_w - (w + UI_DEBUG_GAP_X);
            int y = gui->window_h - (h + UI_DEBUG_GAP_X);
			al_draw_filled_rounded_rectangle(x, y, x+w, y+h,
				8, 8, al_map_rgba(0, 0, 0, 200));
			al_draw_textf(gui->font, al_map_rgb(255, 255, 255),
	            x+(w/2), y+UI_TEXT_GAP_Y, ALLEGRO_ALIGN_CENTRE, "FPS: %d", gui->fps);
		}
    }
	
    al_flip_display();
    gui->fps_accum++;
    if (t - gui->fps_time >= 1) {
        gui->fps = gui->fps_accum;
        gui->fps_accum = 0;
        gui->fps_time = t;
    }
}

int gui_is_within(int x, int y, int xx, int yy, int w, int h)
{
    return x >= xx && x < xx + w && y >= yy && y < yy + h;
}

int is_same_event(Event* a, Event* b)
{
    if (a->type != b-> type)
        return 0;

    switch (a->type)
    {
        case EVENT_NONE: return 1;
        case EVENT_TIMER: return 0;
        case EVENT_LEVEL: return a->level_event.mouse_event_type == b->level_event.mouse_event_type &&
                          a->level_event.row == b->level_event.row &&
                          a->level_event.col == b->level_event.col;
        case EVENT_BUTTON: return a->button_event.mouse_event_type == b->button_event.mouse_event_type &&
                           a->button_event.function == b->button_event.function;
        case EVENT_BUILD_SELECTOR: return a->build_selector_event.mouse_event_type == b->build_selector_event.mouse_event_type &&
                          a->build_selector_event.cell_type == b->build_selector_event.cell_type &&
                          a->build_selector_event.owner == b->build_selector_event.owner;
        default: break;
    }
    fprintf(stderr, "WARNING: unhandled event type in is_same_event: %d\n", a->type);
    return 0;
}

void gui_get_next_game_event(Event *ev) {
    Gui* gui = get_gui();
	bool has_event = 0;

	while (!has_event) {
		ALLEGRO_EVENT event;
		al_wait_for_event(gui->queue, &event);
 
		switch (event.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE: {
				ev->button_event.type = EVENT_BUTTON;
                ev->button_event.mouse_event_type = MOUSE_CLICK;
                ev->button_event.function = UI_EXIT;
				has_event = 1;
				break;
			}
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: 
              {
                  gui->mouse_down = 1;
                  //fall-through
              }
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP: 
              {
                  if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                      gui->mouse_down = 0;
                  //fall-through
              }
			case ALLEGRO_EVENT_MOUSE_AXES: 
              {
                MouseEventType mouse_event_type = 
                    (gui->mouse_down || 
                     event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || 
                     event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) 
                    ? MOUSE_CLICK : MOUSE_HOVER;
                int x = event.mouse.x;
                int y = event.mouse.y;

                if (gui_is_within(x,y, gui->level_offset_x, gui->level_offset_y, gui->level_pixel_w, gui->level_pixel_h)) {
                    ev->level_event.type = EVENT_LEVEL;
                    ev->level_event.mouse_event_type = mouse_event_type;
                    ev->level_event.col = (x - gui->level_offset_x) / gui->tile_w;
                    ev->level_event.row = (y - gui->level_offset_y) / gui->tile_h;
                } else {
					if (event.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
						for (int i = 0; i < gui->button_count; i++) {
							GuiButton* button = &gui->buttons[i];
							if (gui_is_within(x, y, button->x, button->y, button->w, button->h)) {
								if (button->is_build_selector) {
									ev->build_selector_event.mouse_event_type = mouse_event_type;
									ev->build_selector_event.type = EVENT_BUILD_SELECTOR;
									ev->build_selector_event.cell_type = button->cell_type;
									ev->build_selector_event.owner = button->owner;
								}
								else {
									ev->button_event.mouse_event_type = mouse_event_type;
									ev->button_event.type = EVENT_BUTTON;
									ev->button_event.function = button->function;
								}
								break;
							}
							ev->type = EVENT_NONE;
						}
					} else
						ev->type = EVENT_NONE;
                }

                if (ev->type == EVENT_NONE || is_same_event(&gui->prev_interaction_event, ev)) {
                    has_event = 0;
                } else {
                    gui->prev_interaction_event = *ev;
                    has_event = 1;
                }

				//catch double clicks
				if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
					gui->prev_interaction_event.type = EVENT_NONE;
				}

				break;
			}
			case ALLEGRO_EVENT_TIMER: {
				ev->timer_event.type = EVENT_TIMER;
				has_event = 1;
				break;
			}
			case ALLEGRO_EVENT_DISPLAY_RESIZE: {
				al_acknowledge_resize(gui->display);
				has_event = 0;
				break;
			}
			default: {
				has_event = 0;
				break;
			}
		}
	}
}

char* gui_show_save_file_dialog()
{
    Gui* gui = get_gui();
    ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
	ALLEGRO_FILECHOOSER * save_dialog =
		al_create_native_file_dialog(
			al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP)/*initial_path*/,
			"Save Map",
			"*.world;*.wld",
                ALLEGRO_FILECHOOSER_SAVE
                );
    al_stop_timer(gui->timer);
    int dialog_success = al_show_native_file_dialog(gui->display, save_dialog);
    al_destroy_path(path);
    al_start_timer(gui->timer);
    if (!dialog_success) {
        al_destroy_native_file_dialog(save_dialog);
        return NULL;
    }

    int file_count = al_get_native_file_dialog_count(save_dialog);
    if (file_count >= 1) {
        const char* filename = al_get_native_file_dialog_path(save_dialog, 0);
        strncpy(gui->dialog_filename, filename, GUI_MAX_FILENAME_LEN);
        al_destroy_native_file_dialog(save_dialog);
        return gui->dialog_filename;
    } else {
        al_destroy_native_file_dialog(save_dialog);
        return NULL;
    }
}

char* gui_show_load_file_dialog()
{
    Gui* gui = get_gui();
    ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    ALLEGRO_FILECHOOSER * load_dialog = 
        al_create_native_file_dialog(
                 al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP)/*initial_path*/,
                "Save Map",
                "*.world;*.wld",
                0 
                );
    al_stop_timer(gui->timer);
    int dialog_success = al_show_native_file_dialog(gui->display, load_dialog);
    al_destroy_path(path);
    al_start_timer(gui->timer);
    if (!dialog_success) {
        al_destroy_native_file_dialog(load_dialog);
        return NULL;
    }
    int file_count = al_get_native_file_dialog_count(load_dialog);
    if (file_count == 1) {
        const char* filename = al_get_native_file_dialog_path(load_dialog, 0);
        strncpy(gui->dialog_filename, filename, GUI_MAX_FILENAME_LEN);
        al_destroy_native_file_dialog(load_dialog);
        return gui->dialog_filename;
    } else {
        al_destroy_native_file_dialog(load_dialog);
        return NULL;
    }
}

