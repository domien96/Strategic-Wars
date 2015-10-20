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

int remove_old_headquarter(Cell cell, Level* level);

int main()
{
    printf("Strategic War Level Editor\n");

    ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, "basic.world");

    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
    Level* level = level_alloc_read_from_file(filename);
    if (level == NULL) {
        fprintf(stdout, "Reading level from \"%s\" failed.\n", filename);
        al_destroy_path(path);
        return 1;
    }
    al_destroy_path(path);

	/* Theme */
	const char* theme = "civ";
	gui_initialize(theme);

    gui_set_level(level);
    
    gui_draw_frame();

	/* Build_selector chosen type and owner (By default: GROUND and OWNER_NONE) */
	CellType chosen_cell_type = GROUND;
	Owner chosen_owner = OWNER_NONE;
	gui_set_build_highlight(chosen_cell_type, chosen_owner);

	/* GAME LOOP */
    int stop = 0;
	while(!stop) {
        Event event;
        gui_get_next_game_event(&event);

        switch (event.type) {
			case EVENT_BUILD_SELECTOR:
			{
				if (event.button_event.mouse_event_type == MOUSE_CLICK) {
					chosen_cell_type = event.build_selector_event.cell_type;
					chosen_owner = event.build_selector_event.owner;
					gui_set_build_highlight(chosen_cell_type, chosen_owner);
				}
				break;
			}
            case EVENT_TIMER:
            {
                gui_draw_frame();
                break;
            }
			case EVENT_LEVEL:
			{
				switch (event.level_event.mouse_event_type) {
					case MOUSE_HOVER:
					{
						int row = event.level_event.row;
						int col = event.level_event.col;
						gui_set_level_highlight(row, col);
						break;
					}
					case MOUSE_CLICK:
					{
						int row = event.level_event.row;
						int col = event.level_event.col;
						Cell new_cell = { row, col, chosen_cell_type,chosen_owner };
						
						/*headquarter is altijs 4 tegels groot*/
						if (chosen_cell_type == HEADQUARTER) {
							if (row >= level->height - 1 || col >= level->width - 1) {
								gui_add_message("Headquarter has size 4, please try again.");
							}
							else {
								remove_old_headquarter(new_cell, level);
								level->cells[row][col] = new_cell;
								level->cells[row + 1][col] = new_cell;
								level->cells[row][col + 1] = new_cell;
								level->cells[row + 1][col + 1] = new_cell;
							}
							//TODO
							/*ook nog vorige headquarter verwijderen, best ergens apart bijhouden*/
						}
						else {
							Cell current_cell = level->cells[row][col];
							if (current_cell.type == HEADQUARTER) {
								gui_add_message("Headquarter must remain size four");
								//TODO headquarter moet verwijderd worden en unit moet geplaatst worden
							}
							else {
								level->cells[row][col] = new_cell;
							}
						}
						break;
					}
				}
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
						case UI_SAVE:
						{
							char* filename = gui_show_save_file_dialog();
							level_write_to_file(level,filename);
							break;
						}
						case UI_LOAD:
						{
							char* filename = gui_show_load_file_dialog();
							Level* new_level = level_alloc_read_from_file(filename);

							/* Indien fout bij inladen, level heeft dan ongeldig formaat*/
							if ( new_level == NULL) {
								gui_add_message("Error: invalid file. Please choose another file");
								break;
							} else {	/* Everything is fine from here */
								/* Free the previous level first*/
								level_free(level);
							}

							/* Assign new level to the level-pointer */
							level = new_level;
							/* Draw */
							gui_set_level(level);
							/*error messages nog wegdoen als er een geldige file gekozen is*/
							gui_clear_messages();
							break;
						}
						case UI_CLEAR:
						{
							/* Free the current level first*/
							level_free(level);
							/* Set net empty level*/
							level = level_alloc_empty();
							/* Draw */
							gui_set_level(level);
							break;
						}
						case UI_THEME:
						{
							if (theme == "civ") {
								theme = "civ_b";
							} else { /* theme == "civ_b" */
								theme = "civ";
							}
							gui_load_theme(theme);
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

/*zoek in level de oude headquarter (van zelfde speler) en verwijder die als die er is*/
/*return 1 als er een headquarter verwijderd is, return 0 anders*/
/*deze functie moet dus voor het plaatsen van de nieuwe headquarter opgeroepen worden*/
int remove_old_headquarter(Cell cell, Level* level) {
	int i, j;

	for (i = 0; i < level->height; i++) {
		for (j = 0; j < level->width; j++) {
			if ((level->cells[i][j]).type == HEADQUARTER) {
				if ((level->cells[i][j]).owner == cell.owner) {
					/*oude headquarter wordt grond en eigenaar wordt OWNER_NONE*/
					(level->cells[i][j]).type = GROUND;
					(level->cells[i+1][j]).type = GROUND;
					(level->cells[i][j+1]).type = GROUND;
					(level->cells[i+1][j+1]).type = GROUND;
					(level->cells[i][j]).owner = OWNER_NONE;
					(level->cells[i + 1][j]).owner = OWNER_NONE;
					(level->cells[i][j + 1]).owner = OWNER_NONE;
					(level->cells[i + 1][j + 1]).owner = OWNER_NONE;
					return 0;
				}
			}
		}
	}
}

