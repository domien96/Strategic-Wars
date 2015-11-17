
#include "editor.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <stdio.h>


int main(int argc, char** argv)
{
	printf("Strategic War Level Editor\n");

	// Check if the command line argument is of the form "GuilLevelEditor.exe width height"
	int arg_w=0;
	int arg_h=0;
	bool dimarg_given = false;
	// We also check if these dimensions are valid. If they aren't or there are none given, we will use the default dimensions to construct the level.
	if (argc >= 3) {
		dimarg_given = (sscanf_s(argv[1], "%i", &arg_w) == 1) && (sscanf_s(argv[2], "%i", &arg_h) == 1);
	} 

	/* Create a new level */
	Level* level = make_new_level(dimarg_given, arg_w, arg_h);

	/* Theme */
	const char* theme = THEME;
	gui_initialize(theme);

    gui_set_level(level);
    
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

						/* verwijder eventueel onderliggende HQ*/
						if (clicked_cell.type == HEADQUARTER) {
							remove_old_headquarter((level->cells[row][col]).owner, level);
						}
						
						/* verwijder andere HQ van hetzelfde team (indien er een was) */
						if (chosen_cell_type == HEADQUARTER) {
							/*headquarter is altijd 4 tegels groot*/
							if (row >= level->height - 1 || col >= level->width - 1) {
								gui_add_message("Headquarter has size 4, please try placing it somewhere else.");
							} else {
								remove_old_headquarter(new_cell.owner, level);
								/*alle tegels controleren of er geen andere headquarter staat
								* tegel linksboven wordt sowieso gecontroleerd
								*/
								if ((level->cells[row + 1][col]).type == HEADQUARTER) {
									remove_old_headquarter(level->cells[row + 1][col].owner, level);
								}
								else if ((level->cells[row][col + 1]).type == HEADQUARTER) {
									remove_old_headquarter(level->cells[row][col + 1].owner, level);
								}
								else if ((level->cells[row + 1][col + 1]).type == HEADQUARTER) {
									remove_old_headquarter(level->cells[row + 1][col + 1].owner, level);
								}
								/* Change to new cells and updating coordinates also (see previous bug)*/
								level->cells[row][col] = new_cell;

								level->cells[row + 1][col]=new_cell;
								level->cells[row + 1][col].row += 1;

								level->cells[row][col + 1] = new_cell;
								level->cells[row][col + 1].col += 1;

								level->cells[row + 1][col + 1] = new_cell;
								level->cells[row + 1][col + 1].row += 1;
								level->cells[row + 1][col + 1].col += 1;
							}
						} else {
							/* Normale geval */
							level->cells[row][col] = new_cell;
						}

						/* Pad opnieuw berekenen */
						updatePath(level);
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
							int status = level_write_to_file(level, filename);
							if (status == 0) {
								gui_add_message("Level has been succesfully saved.");
							}
							else if (status == 2) {
								gui_add_message("Error: Cannot access the chosen file");
							}
							else if (status == 3) {
								gui_add_message("No file is chosen. Nothing has been done.");
							}
							break;
						}
						case UI_LOAD:
						{
							char* filename = gui_show_load_file_dialog();
							Level* new_level = level_alloc_read_from_file(filename);

							if ( new_level == NULL) {
								/* Indien een fout bij inladen optreedt, heeft level een ongeldig formaat */
								gui_add_message("Error: invalid file. Please choose another file");
								break;
							} else {	
								/* Everything is fine from here */
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

							/* Create a new level */
							level = make_new_level(dimarg_given, arg_w, arg_h);

							/* Draw */
							gui_set_level(level);
							updatePath(level);
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


int remove_old_headquarter(Owner owner, Level* level) {
	int i, j;
	for (i = 0; i < level->height; i++) {
		for (j = 0; j < level->width; j++) {
			if ((level->cells[i][j]).type == HEADQUARTER) {
				if ((level->cells[i][j]).owner == owner) {
					/*oude headquarter wordt naar DEFAULT waarden gezet. */
					(level->cells[i][j]).type = DEFAULT_CELLTYPE;
					(level->cells[i + 1][j]).type = DEFAULT_CELLTYPE;
					(level->cells[i][j + 1]).type = DEFAULT_CELLTYPE;
					(level->cells[i + 1][j + 1]).type = DEFAULT_CELLTYPE;
					(level->cells[i][j]).owner = DEFAULT_OWNER;
					(level->cells[i + 1][j]).owner = DEFAULT_OWNER;
					(level->cells[i][j + 1]).owner = DEFAULT_OWNER;
					(level->cells[i + 1][j + 1]).owner = DEFAULT_OWNER;
					return 0;
				}
			}
		}
	}
	return 1;
}


int updatePath(Level* level) {
	/* Linksboven cells van HQ */
	Cell *humanHQ = (Cell*)NULL, *aiHQ = (Cell*)NULL;

	/* Zijn er 2 verschillende HQ's? */
	for (int i = 0; i < level->height; i++) {

		if (humanHQ && aiHQ) {
			/* If both found, we don't need to look further.*/
			break;
		}

		for (int j = 0; j < level->width; j++) {
			if (humanHQ && aiHQ) {
				/* If both found, we don't need to look further.*/
				break;
			}

			Cell* current = &level->cells[i][j];
			if (current->type == HEADQUARTER) {
				if (humanHQ == NULL && current->owner == OWNER_HUMAN) {
					humanHQ = current;
				}
				else if (aiHQ == NULL && current->owner == OWNER_AI) {
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
		//path checken voor het geheugen vrijgegeven wordt, dit was de bug in deel 1
		/*error message als path kleiner is dan 100*/
		if (path->distance < 100) {
			gui_add_message("ERROR: Headquarters are too close: minimum distance is 100");
			return 3;
		}
		else {
			return 0;
		}
		gui_show_path(path);
		path_free(path);
	}
	else {
		gui_add_message("Path between headquarters is unexistent. Please try placing somewhere else.");
		gui_show_path(NULL);
		return 2;
	}
}


Level* make_new_level(bool dimarg_given, int arg_w, int arg_h) {
	if (dimarg_given) {
		return level_alloc_empty_with_dim(arg_w, arg_h);
	}
	return level_alloc_empty();
}
