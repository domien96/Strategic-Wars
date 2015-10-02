#include "level.h"
#include "pathfinder.h"
#include "gui.h"
#include "common.h"
#include "event.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <stdio.h>

int main()
{
    printf("Strategic War Level Editor\n");

    ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, "basic.world");

    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
    Level* level = level_alloc_read_from_file(filename);
    //TODO: uncomment this 
    //if (level == NULL) {
    //    fprintf(stdout, "Reading level from \"%s\" failed.\n", filename);
    //    al_destroy_path(path);
    //    return 1;
    //}
    al_destroy_path(path);

    //gui_initialize("civ_b");
    gui_initialize("civ");

    gui_set_level(level);
    
    gui_draw_frame();
    int stop = 0;
	while(!stop) {
        Event event;
        gui_get_next_game_event(&event);

        switch (event.type) {
            case EVENT_TIMER:
                {
                    gui_draw_frame();
                    break;
                }
            case EVENT_BUTTON: 
                {
                    if (event.button_event.mouse_event_type == MOUSE_CLICK) {
                        switch (event.button_event.function) {
                            case UI_EXIT:
                                {
                                    stop = 1;
                                    break;
                                }
                            default: break;
                                        }
                                    }
                                    break;
                                }
            default: break;
        }
	}

	gui_free();
    level_free(level);

    return 0;
}

