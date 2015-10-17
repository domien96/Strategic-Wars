#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"


// MACROS

/*Make sure these 3 properties (of a Cell) are consistent !*/
#define DEFAULT_SYMBOL '*'
#define DEFAULT_CELLTYPE GROUND
#define DEFAULT_OWNER OWNER_NONE

#define LEVEL_MAX_WIDTH 25
#define LEVEL_MAX_HEIGHT 12

/*
 * This struct represents a single cell in a level.
 * Each cell has a position (row, col), an owner and a type.
 */
typedef struct Cell 
{
    int row;
    int col;
    CellType type;
    Owner owner;
} Cell;

/*
 * This struct represents a Level
 *
 * Note that no pointer is used for the "cells" array.
 * This means that the cells are all stored inside the struct itself.
 * In other words, the memory used to store the Level struct, 
 * contains all the cells directly, it doesn't point to other memory 
 * containing the cells.
 */
typedef struct Level 
{
    Cell cells[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];
    int width;
    int height;
} Level;

/*
 * This function converts the textual representation of a Cell,
 * which is used in the Level file format,
 * to a CellType.
 * If symbol is not found, this function returns DEFAULT_CELLTYPE
 *
 * For example, it converts the char '*' to the CellType GROUND
 */
CellType level_symbol_to_cell_type(char symbol);
/*
 * This function converts the textual representation of a Cell,
 * which is used in the Level file format,
 * to an owner.
 * If symbol is not found, this function returns DEFAULT_OWNER
 *
 * For example, it converts the char 'h' to the Owner HUMAN_PLAYER
 */
Owner level_symbol_to_owner(char symbol);

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
 * If celltype or owner is not found, this function returns DEFAULT_SYMBOL.
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
int level_is_valid_pos(Level* level, int row, int col);

/*
 * This functions returns pointer to an empty newly allocated level.
 * Each cell will be set to DEFAULT.
 */
Level* level_alloc_empty();
/*
 * This functions returns pointer to an newly allocated level.
 * The level is initialised by reading and processing the given file.
 */
Level* level_alloc_read_from_file(const char* filename);
/*
 * Free the memory used by a level.
 */
void level_free(Level* level);

/*
 * This functions stores the given level to file.
 * Returns 1 : when file cannot be found.
 *		   2 : when writing to file is not allowed
 */
int level_write_to_file(Level* level, const char* filename);

#endif

