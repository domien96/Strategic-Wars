#include "level.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

//TODO: implement all function from level.h
/*
* This function converts the textual representation of a Cell,
* which is used in the Level file format,
* to a CellType.
*
* For example, it converts the char '*' to the CellType GROUND
*/
CellType level_symbol_to_cell_type(char symbol);
/*
* This function converts the textual representation of a Cell,
* which is used in the Level file format,
* to an owner.
*
* For example, it converts the char 'h' to the Owner HUMAN_PLAYER
*/
Owner level_symbol_to_owner(char symbol) {
	switch (symbol) {
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
char cell_to_symbol(Cell* cell);

/*
* This function converts a combination of an owner and cell_type
* to the textual representation of the corresponding cell in the
* level file format.
*
* for example, it converts the combination UNIT_2, OWNER_AI
* to the character '8'
*/
char cell_type_and_owner_to_symbol(CellType cell_type, Owner owner);

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
int level_can_walk_over(Cell* unit, Cell* target);

/*
* This function returns true if the given position is a valid cell
* position within the given level.
* A position is valid when it is within the dimensions of the level.
* Note: Take negative row and coloumn values into account (they are always invalid)
*/
int level_is_valid_pos(Level* level, int row, int col) {

}

/*
* This functions stores the given level to file.
*/
int level_write_to_file(Level* level, const char* filename);

/*
* This functions returns pointer to an empty newly allocated level.
*/
Level* level_alloc_empty()
{
	Level* lvlP = (Level*) malloc(sizeof(Level));
	return lvlP;
		
}

Level* level_alloc_read_from_file(const char* filename)
{
    //TODO
        return NULL;
}

void level_free(Level* level)
{
    //TODO
}
