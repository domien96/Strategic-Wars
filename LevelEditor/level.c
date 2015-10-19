#include "level.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

/////////////////////////////////////////////////////////////

// (PRIVATE) METHODS
/*
* Initialises the given level with the data inside the given file.
* Returns 0 when everything went fine.
* File needs the minimal length for each line, otherwise this function will return 1.
*/
int init_level(Level* level, FILE* fp);


/////////////////////////////////////////////////////////////

/*
* This function converts the textual representation of a Cell,
* which is used in the Level file format,
* to a CellType.
* If symbol is not found, this function returns DEFAULT_CELLTYPE.
*
* For example, it converts the char '*' to the CellType GROUND
*/
CellType level_symbol_to_cell_type(char symbol) {
	switch (symbol) {
	default:
		return DEFAULT_CELLTYPE; 
	case '*': 
		return GROUND;
		break;
	case 'W':
		return WATER;
		break;
	case 'B':
		return BRIDGE;
		break;
	case 'R':
		return ROCK;
		break;
	case 'H':
		return HEADQUARTER;
		break;
	case 'h' :
		return HEADQUARTER;
		break;
	case '1':
		return UNIT_1;
		break;
	case '2':
		return UNIT_2;
		break;
	case '3':
		return UNIT_3;
		break;
	case '7':
		return UNIT_1;
		break;
	case '8':
		return UNIT_2;
		break;
	case '9':
		return UNIT_3;
		break;
	}

}
/*
* This function converts the textual representation of a Cell,
* which is used in the Level file format,
* to an owner.
* If symbol is not found, this function returns DEFAULT_OWNER
*
* For example, it converts the char 'h' to the Owner HUMAN_PLAYER
*/
Owner level_symbol_to_owner(char symbol) {
	switch (symbol) {
	/*default*/
	default:
		return DEFAULT_OWNER;
		break;
		/*owner_none*/
	case '*':
		return OWNER_NONE;
		break;
	case 'W':
		return OWNER_NONE;
		break;
	case 'B':
		return OWNER_NONE;
		break;
	case 'R':
		return OWNER_NONE;
		break;
		/*owner_human*/
	case 'h':
		return OWNER_HUMAN;
		break;
	case '1':
		return OWNER_HUMAN;
		break;
	case '2':
		return OWNER_HUMAN;
		break;
	case '3':
		return OWNER_HUMAN;
		break;
		/*owner_ai*/
	case 'H':
		return OWNER_AI;
		break;
	case '7':
		return OWNER_AI;
		break;
	case '8':
		return OWNER_AI;
		break;
	case '9':
		return OWNER_AI;
		break;
	}
}

/*
* This function converts a Cell to the textual representation of
* that cell in the level file format.
*/
char cell_to_symbol(Cell* cell) {
	return cell_type_and_owner_to_symbol(cell->type, cell->owner); /* hier zou ik pointers willen doorgeven aangezien die sneller gekopi�erd worden,
																   maar ik mag de header niet wijzigen, veronderstel ik.*/
}

/*
* This function converts a combination of an owner and cell_type
* to the textual representation of the corresponding cell in the
* level file format.
* 
* If celltype or owner is not found, this function returns DEFAULT_SYMBOL.
*
* for example, it converts the combination UNIT_2, OWNER_AI
* to the character '8'
*/
char cell_type_and_owner_to_symbol(CellType cell_type, Owner owner) {
	/* code smell*/
	switch (owner) {
	default:
		return DEFAULT_SYMBOL; 
		break;
	case OWNER_HUMAN:

		switch (cell_type) {
		case HEADQUARTER:
			return 'h';
			break;
		case UNIT_1:
			return '1';
			break;
		case UNIT_2:
			return '2';
			break;
		case UNIT_3:
			return '3';
			break;
		default:
			return DEFAULT_SYMBOL;
			break;
		}
		break;

	case OWNER_AI:

		switch (cell_type) {
		case HEADQUARTER:
			return 'H';
			break;
		case UNIT_1:
			return '7';
			break;
		case UNIT_2:
			return '8';
			break;
		case UNIT_3:
			return '9';
			break;
		default:
			return DEFAULT_SYMBOL;
			break;
		}
		break;

	case OWNER_NONE:

		switch (cell_type) {
		case GROUND:
			return '*';
			break;
		case WATER:
			return 'W';
			break;
		case BRIDGE:
			return 'B';
			break;
		case ROCK:
			return 'R';
			break;
		default:
			return DEFAULT_SYMBOL;
			break;
		}
		break;

	}
}

/*
* This functions returns true if the unit in a given Cell can
* move over the given target Cell.
* A special exception is made if the unit is a HEADQUARTER.
* In that case, the headquerter must be considered to be able to
* cross units. This is convenient when checking if a path between 2
* headquarters exists.
*
* When a non ownable "unit" is passed, the output of this function is undefined.
*
* (No pathfinding is used here, only the CellType is important)
*
* Some examples:
*   return | unit CellType | target CellType
*     0    |      UNIT_2   |     ROCK
*     1    |      UNIT_3   |     GROUND
*     0    |      UNIT_1   |     UNIT_1
*     0    |   HEADQUARTER |     WATER
*     1    |   HEADQUARTER |     BRIDGE
*     1    |   HEADQUARTER |     UNIT_1
*/
int level_can_walk_over(Cell* unit, Cell* target) {
	/*"When a non ownable "unit" is passed, the output of this function is undefined."*/
	if (! cell_type_is_player_owned(unit->type)) {
		return 0;
	}

	/* Units cannot cross other units.
	 * "A special exception is made if the unit is a HEADQUARTER. ..." 
	 * This "exception" is implied by the next section. */
	if (cell_type_is_unit(target->type)) {
		if (cell_type_is_unit(unit->type))
			return 0;
		else if (unit->type == HEADQUARTER)
			return 1;
	}

	/* Unable to cross water or rocks*/
	if (target->type == WATER || target->type == ROCK) {
		return 0;
	}

	/* ALL OTHER COMBINATIONS ARE LEGAL */
	return 1;
}

/*
* This function returns true if the given position is a valid cell
* position within the given level.
* A position is valid when it is within the dimensions of the level.
* Note: Take negative row and column values into account (they are always invalid)
*/
int level_is_valid_pos(Level* level, int row, int col) {
	/* Negatives => invalid**/
	if (row < 0 || col < 0) {
		return 0;
	}

	/* check if between 0 and size-1*/
	if (row >= level->height || col >= level->width) {
		return 0;
	}
	/*PASSED ALL ELIMINATIONS ABOVEBOARD*/
	return 1;
}

/*
* This functions stores the given level to file.
* Returns 1 : when file cannot be found.
*		  2 : when writing to file is not allowed
*		  3 : filename was null
*/
int level_write_to_file(Level* level, const char* filename) {
	if (!filename) {
		return 3;
	}
	/* open() clears all the data if the file already exists*/
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		return 1;
	}

	/* Write dimensions*/
	if (fprintf(fp, "%i|%i\n", level->width, level->height) < 0) {
		return 2;
	}

	/* Write each row (as symbols)*/
	for (int r = 0; r < level->height; r++) {
		for (int c = 0; c < level->width; c++) {
			Cell cell = level->cells[r][c];
			char symbol = cell_to_symbol(&cell);
			if (fprintf(fp, "%c", symbol) < 0) {
				return 2;
			}
		}
		if (fprintf(fp, "\n") < 0) {
			return 2;
		}
	}
	/*Close file*/
	fclose(fp);

	return 0;
}

/*
* This functions returns pointer to an empty newly allocated level.
* Each cell will be set to DEFAULT.
*/
Level* level_alloc_empty()
{
	/* Allocate struct*/
	Level* lvlP = (Level*) malloc(sizeof(Level));

	/* Init width and height*/
	int width = LEVEL_MAX_WIDTH, height = LEVEL_MAX_HEIGHT; /* Always max*/
	lvlP->height = height;
	lvlP->width = width;

	/* Init cells : Fill up with cells*/
	Cell cell = { 0 , 0 , DEFAULT_CELLTYPE, DEFAULT_OWNER };/* Sample cell */
	for (int r = 0; r < height; r++) {
		cell.row = r;
		for (int c = 0; c < width; c++) {
			cell.col = c;
			lvlP->cells[r][c] = cell;
		}
	}
	return lvlP;
		
}

Level* level_alloc_read_from_file(const char* filename)
{
	if (!filename) {
		return (Level*)NULL;
	}
    // init empty level
	Level* level = level_alloc_empty();

	/* open() the file */
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		return (Level*) NULL;
	}

	/*init level*/
	if (init_level(level, fp)) {
		return (Level*)NULL;
	}	
	
	/*Close file*/
	fclose(fp);
    
	return level;
}

/*
 * Initializes the given level with the data inside the given file.
 * Returns 0 when everything went fine.
 * File needs the minimal length for each line, otherwise this function will return 1.
 */
int init_level(Level* level, FILE* fp) {
	/* Initializes dimensions*/
	int height=0, width=0;
	fscanf(fp, "%i|%i\n", &width, &height);
	/* Check dimensions */
	if (width <= 0 || height <= 0) {
		return 1;
	};
	level->height = height;
	level->width = width;


	/* Initializes cells, opletten dat er juist 'width' characters gelezen worden per line*/
	char* buffer = malloc((LEVEL_MAX_WIDTH+1)*sizeof(int));
	for (int r = 0; r < level->height; r++) {
		if (!fgets(buffer, LEVEL_MAX_WIDTH+2, fp)) { 
			return 1;
		}
		/*lijn omzetten*/
		//Cell current_row[] = level->cells[r];
		for (int c = 0; c < level->width; c++) {
			char symbol = *(buffer+c);
			/* init cell */
			Cell* cell = malloc(sizeof(Cell)); 
			cell->row = r;
			cell->col = c;
			cell->type=level_symbol_to_cell_type(symbol);
			cell->owner=level_symbol_to_owner(symbol);
			/* place cell */
			level->cells[r][c] = *cell;
			free(cell);
		}
	}
	//rewind(fp); // help
	free(buffer);
	
	return 0;
}


void level_free(Level* level)
{
    /* No direct/non-direct dynamic allocated memory by level's contents */
	free(level);
}
