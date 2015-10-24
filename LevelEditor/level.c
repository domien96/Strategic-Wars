
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "level.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <string.h>


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
	return cell_type_and_owner_to_symbol(cell->type, cell->owner); /* hier zou ik pointers willen doorgeven aangezien die sneller gekopiëerd worden,
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
	/* Units cannot cross other units.
	 * "A special exception is made if the unit is a HEADQUARTER. ..." 
	 * This "exception" is implied by the next section. */
	if (cell_type_is_unit(target->type) || target->type == HEADQUARTER) {
		if (unit->type == HEADQUARTER)
			return 1;
		else
			return 0;
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
* Returns 
*		  2 : when writing to file is not allowed
*		  3 : filename was null
*/
int level_write_to_file(Level* level, const char* filename) {

	if (!filename) {
		return 3;
	}
	else if (strlen(filename) < 6) {
		return 2;
	}

	// Check the extension
	char* extension = (char*)calloc(8, sizeof(char));
	memcpy(extension, &filename[strlen(filename) - 4], 5);
	if (!strcmp(extension,".wld")) {
		return level_write_to_file_binary(level, filename);
	}
	else {
		if (strlen(filename) < 8) {
			return 2;
		}
		memcpy(extension, &filename[strlen(filename) - 6], 7);
		if (strcmp(extension,".world")) {
			return 2;
		}
	}
	free(extension);

	/* open() clears all the data if the file already exists*/
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		return 2;
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
* This functions stores the given level to file.
* Returns 1 : when file cannot be found.
*		  2 : when writing to file is not allowed
*		  3 : filename was null
*/
int level_write_to_file_binary(Level* level, const char* filename) {
	if (!filename) {
		return 3;
	}
	/* open() clears all the data if the file already exists*/
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		return 1;
	}

	fprintf(fp, ".SOI");

	/* Write version */
	fprintf(fp, "%c", (unsigned char) 1);

	/* Write dimensions*/
	unsigned short width = (unsigned short) level->width;
	unsigned char w0 = (unsigned char)(width >> 8);
	unsigned char w1 = (unsigned char)width;
	unsigned short height = (unsigned short) level->height;
	unsigned char h0 = (unsigned char)(height >> 8);
	unsigned char h1 = (char)height;

	if (fprintf(fp, "%c%c%c%c", w0, w1, h0, h1) < 0) {
		return 2;
	}

	/* Write each row (as symbols)*/
	for (int r = 0; r < level->height; r++) {
		for (int c = 0; c < level->width; c++) {
			Cell cell = level->cells[r][c];
			
			char ch = 0;
			switch (cell.owner) {
			case OWNER_NONE:
				break;
			case OWNER_HUMAN:
				ch = 1;
				break;
			case OWNER_AI:
				ch = 2;
				break;
			default:
				break;
			}

			ch <<= 3;
			switch (cell.type) {
			case GROUND:
				break;
			case WATER:
				ch += 1;
				break;
			case ROCK:
				ch += 2;
				break;
			case HEADQUARTER:
				ch += 3;
				break;
			case BRIDGE:
				ch += 4;
				break;
			default:
				break;
			}

			ch <<= 1;
			//There are no roads, so do nothing

			ch <<= 2;
			switch (cell.type) {
			case UNIT_1:
				ch += 1;
				break;
			case UNIT_2:
				ch += 2;
				break;
			case UNIT_3:
				ch += 3;
				break;
			default:
				break;
			}

			if (fprintf(fp, "%c", ch) < 0) {
				return 2;
			}
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
	/* Init width and height*/
	int width = LEVEL_MAX_WIDTH, height = LEVEL_MAX_HEIGHT; /* Always max*/
	return level_alloc_empty_with_dim(width, height);
}


/*
* This function returns a pointer to an empty newly allocated level with the
* given width and the given height.
* Each cell will be set to DEFAULT.
* Invalid parameters (not between 1 and MAX) will result into their maximal values.
*/
Level* level_alloc_empty_with_dim(int width, int height) {
	/* Allocate struct*/
	Level* lvlP = (Level*)malloc(sizeof(Level));

	/* kleiner dan 1 corrigeren */
	if (height < 1) {
		height = LEVEL_MAX_HEIGHT;}
	if (width < 1) {
		width = LEVEL_MAX_WIDTH;}

	/* groter dan max corrigeren */
	lvlP->height = min(LEVEL_MAX_HEIGHT,height);
	lvlP->width = min(LEVEL_MAX_WIDTH,width);

	/* Init cells : Fill up with cells*/
	Cell cell = { 0 , 0 , DEFAULT_CELLTYPE, DEFAULT_OWNER };/* Sample cell */
	for (int r = 0; r < lvlP->height; r++) {
		cell.row = r;
		for (int c = 0; c < lvlP->width; c++) {
			cell.col = c;
			lvlP->cells[r][c] = cell;
		}
	}
	return lvlP;
}


Level* level_alloc_read_from_file(const char* filename)
{
	if (!filename || strlen(filename) < 6) {
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
	char extension[5];
	memcpy(extension, &filename[strlen(filename) - 4], 5);
	if (!strcmp(extension,".wld")) {
		if (init_level_binary(level, fp)) {
			return (Level*)NULL;
		}
	}
	else {
		if (strlen(filename) < 8) {
			return (Level*)NULL;
		}
		char extension0[7];
		memcpy(extension0, &filename[strlen(filename) - 6], 7);
		if (strcmp(extension0, ".world")) {
			return (Level*)NULL;
		}
		if(init_level(level, fp)) {
			return (Level*)NULL;
		}
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

int init_level_binary(Level* level, FILE* fp) {
	if (fscanf(fp, ".SOI")) {
		return 1;
	}
	/* Initializes dimensions*/
	unsigned char version;
	unsigned char d00, d01, d10, d11;
	if (fscanf(fp, "%c", &version) != 1) {
		return 1;
	}
	if (fscanf(fp, "%c%c%c%c", &d00, &d01, &d10, &d11) != 4) {
		return 1;
	}

	int width = (int) (((short)d00) << 8) | d01;
	int height = (int) (((short)d10) << 8) | d11;

	/* Check dimensions */
	if (width <= 0 || height <= 0) {
		return 1;
	};
	level->height = height;
	level->width = width;

	/* Initializes cells, opletten dat er juist 'width' characters gelezen worden per line*/
	char* buffer = malloc((LEVEL_MAX_WIDTH + 1)*sizeof(int));
	for (int r = 0; r < level->height; r++) {
		if (!fgets(buffer, LEVEL_MAX_WIDTH + 1, fp)) {
			return 1;
		}
		/*lijn omzetten*/
		//Cell current_row[] = level->cells[r];
		for (int c = 0; c < level->width; c++) {
			char ch = *(buffer + c);
			/* init cell */
			Cell* cell = malloc(sizeof(Cell));
			cell->row = r;
			cell->col = c;

			int type_number = (ch & 0b00111000) >> 3;
			switch (type_number) {

			case 0:
				cell->type = GROUND;
				break;
			case 1:
				cell->type = WATER;
				break;
			case 2:
				cell->type = ROCK;
				break;
			case 3:
				cell->type = HEADQUARTER;
				break;
			case 4:
				cell->type = BRIDGE;
				break;
			default:
				cell->type = DEFAULT_CELLTYPE;
				break;
			}

			int owner_number = (ch & 0b11000000) >> 6;
			switch (owner_number) {

			case 0:
				cell->owner = OWNER_NONE;
				break;
			case 1:
				if (cell->type == GROUND || cell->type == HEADQUARTER) {
					cell->owner = OWNER_HUMAN;
				}
				else {
					cell->owner = OWNER_NONE;
					cell->type = GROUND;
				}
				break;
			case 2:
				if (cell->type == GROUND || cell->type == HEADQUARTER) {
					cell->owner = OWNER_AI;
				}
				else {
					cell->owner = OWNER_NONE;
					cell->type = GROUND;
				}
				break;
			default:
				cell->owner = DEFAULT_OWNER;
				cell->type = DEFAULT_CELLTYPE;
				break;
			}

			int unit_number = ch & 0b00000011;
			switch (unit_number) {

			case 0:
				break;
			case 1:
				cell->type == GROUND && cell->owner != OWNER_NONE ? (cell->type = UNIT_1) : (cell->type = GROUND);
				break;
			case 2:
				cell->type == GROUND && cell->owner != OWNER_NONE ? (cell->type = UNIT_2) : (cell->type = GROUND);
				break;
			case 3:
				cell->type == GROUND && cell->owner != OWNER_NONE ? (cell->type = UNIT_3) : (cell->type = GROUND);
				break;
			default:
				cell->type = DEFAULT_CELLTYPE;
				break;
			}

			/* place cell */
			level->cells[r][c] = *cell;
			free(cell);
		}
	}
	free(buffer);

	return 0;
}


void level_free(Level* level)
{
    /* No direct/non-direct dynamic allocated memory by level's contents */
	free(level);
}
