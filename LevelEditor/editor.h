#include "level.h"
#include "pathfinder.h"
#include "gui.h"
#include "common.h"
#include "event.h"

int main(int argc, char** argv);

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
		/*error message als path kleiner is dan 100*/
		if (path->distance < 100) {
			gui_add_message("ERROR: Headquarters are too close: minimum distance is 100");
		}
		gui_show_path(path);
		return 0;
	}
	else {
		gui_show_path(NULL);
		return 2;
	}
}

/* Set net empty level.
* If a command line argument is given of the form "width height", then
* a level of dimension width x height will be made when clear is pressed.
* If an invalid argument is given, or there is none, a level of the default dimensions
* is set. We have already checked this in the boolean arg_given and we have saved the dimensions in the
* variables arg_w and arg_h, so this is not checked each time the clear button is pressed.
*/
Level* make_new_level(bool dimarg_given, int arg_w, int arg_h) {
	if (dimarg_given) {
		return level_alloc_empty_with_dim(arg_w, arg_h);
	}
	return level_alloc_empty();
}