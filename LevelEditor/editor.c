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

int remove_old_headquarter(Owner owner, Level* level);

int main(int argc, char** argv)
{
    printf("Strategic War Level Editor\n");

	// Check if a command line argument is given and if it has a legitimate form "width height".
	int arg_w = 0;
	int arg_h = 0;
	bool arg_given = (argc == 2) && (sscanf(argv[0], "%i", &arg_w) == 1) && (sscanf(argv[1], "%i", &arg_h) == 1) && (arg_w>0) && (arg_h>0) && (arg_w<=LEVEL_MAX_WIDTH) && (arg_h<=LEVEL_MAX_HEIGHT);

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
	updatePath(level);
    
    gui_draw_frame();

	/* Build_selector chosen type and owner (By default: GROUND and OWNER_NONE) */
	CellType chosen_cell_type = DEFAULT_CELLTYPE;
	Owner chosen_owner = DEFAULT_OWNER;
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
						Cell clicked_cell = level->cells[row][col];
						Cell new_cell = { row, col, chosen_cell_type,chosen_owner };

						/*indien er op een headquarter geplaatst moet deze verwijderd worden*/
						if (clicked_cell.type == HEADQUARTER) {
							remove_old_headquarter((level->cells[row][col]).owner, level);
						}
						
						
						if (chosen_cell_type == HEADQUARTER) {
							/*headquarter is altijs 4 tegels groot*/
							if (row >= level->height - 1 || col >= level->width - 1) {
								gui_add_message("Headquarter has size 4, please try placing it somewhere else.");
							}
							else {
								remove_old_headquarter(new_cell.owner, level);
								level->cells[row][col] = new_cell;
								level->cells[row + 1][col] = new_cell;
								level->cells[row][col + 1] = new_cell;
								level->cells[row + 1][col + 1] = new_cell;
							}
						} else {
							/* Normale geval */
							level->cells[row][col] = new_cell;
						}

						/* Pad opnieuw berekenen */
						if (updatePath(level) == 2) {
							gui_add_message("Path between headquarters is unexistent. Please try placing somewhere else.");
						};
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
							updatePath(level);
							/*error messages nog wegdoen als er een geldige file gekozen is*/
							gui_clear_messages();
							break;
						}
						case UI_CLEAR:
						{
							/* Free the current level first*/
							level_free(level);
							/* Set net empty level.
							 * If a command line argument is given of the form "width height", then
							 * a level of dimension width x height will be made when clear is pressed.
							 * If an invalid argument is given, or there is none, a level of the default dimensions
							 * is set. We have already checked this in the boolean arg_given and we have saved the dimensions in the
							¨* variables arg_w and arg_h, so this is not checked each time the clear button is pressed.
							 */
							if (arg_given) {
								printf("Dimensions set to:  %i x %i", arg_w, arg_h);
								level = level_alloc_empty_with_dim(arg_w, arg_h);
							}
							else {
								level = level_alloc_empty();
							}
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
/*return 0 als er een headquarter verwijderd is, return 1 anders*/
/*deze functie moet dus voor het plaatsen van de nieuwe headquarter opgeroepen worden*/
int remove_old_headquarter(Owner owner, Level* level) {
	int i, j;
	for (i = 0; i < level->height; i++) {
		for (j = 0; j < level->width; j++) {
			if ((level->cells[i][j]).type == HEADQUARTER) {
				if ((level->cells[i][j]).owner == owner) {
					/*oude headquarter wordt naar DEFAULT waarden gezet. */
					(level->cells[i][j]).type = DEFAULT_CELLTYPE;
					(level->cells[i+1][j]).type = DEFAULT_CELLTYPE;
					(level->cells[i][j+1]).type = DEFAULT_CELLTYPE;
					(level->cells[i+1][j+1]).type = DEFAULT_CELLTYPE;
					(level->cells[i][j]).owner = DEFAULT_OWNER;
					(level->cells[i + 1][j]).owner = DEFAULT_OWNER;
					(level->cells[i][j + 1]).owner = DEFAULT_OWNER;
					(level->cells[i + 1][j + 1]).owner = DEFAULT_OWNER;
					return 0;
				}
			}
		}
		return 1;
	}
}

/* Indien er 2 HeadQuarters (van de verschillende teams) geplaatst zijn en er dus 
 * een pad zou kunnen bestaan tussen de 2 hoofdkwartieren, wordt dit pad berekend.
 * Indien een pad bestaat, wordt dit getekend op de editor. Zoniet wordt er het
 * vorige pad weggehaald. 
 *
 * Returns 0 : Everything is OK.
 *		   1 : There aren't 2 headquarters.
 *		   2 : There are 2 headquarters, but there is no possible path.
 *
 * @author : Domien
 */
int updatePath(Level* level) {
	/* Linksboven cells van HQ */
	Cell *humanHQ = (Cell*) NULL, *aiHQ = (Cell*) NULL;
	
	/* Zijn er 2 verschillende HQ's? */
	for (int i = 0; i < level->height; i++) {
		for (int j = 0; j < level->width; j++) {
			Cell* current = &level->cells[i][j];
			if (current->type == HEADQUARTER) {
				if (current->owner == OWNER_HUMAN) {
					humanHQ = current;
				}
				else if (current->owner == OWNER_AI) {
					aiHQ = current;
				}
			}
		}
	}
	
	if (humanHQ == NULL || aiHQ == NULL) {
		gui_show_path(NULL);
		return 1;
	}

	/* Check if path between HQ's exists. */
	Path* path = find_path(level, humanHQ, aiHQ);
	if (path) {
		gui_show_path(path);
		return 0;
	} else {
		gui_show_path(NULL);
		return 2;
	}
}