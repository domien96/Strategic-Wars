#ifndef COMMON_H
#define COMMON_H

/*
 * Thus enum lists all possible cell types.
 *
 * Note that CELL_TYPE_COUNT is not a CellType.
 * It is the last item in the enum, and thus it is automatically the number of items before it.
 * This can be used when a loop needs to know how many cell types exists.
 * Whenever a cell type is added, it has to be added before CELL_TYPE_COUNT.
 * That way, code using CELL_TYPE_COUNT will work automatically with the new CellType.
 * This trick is often used in C programs.
 */
typedef enum { GROUND, WATER, ROCK, HEADQUARTER, BRIDGE, UNIT_1, UNIT_2, UNIT_3, CELL_TYPE_COUNT } CellType;

/*
 * This enum lists the possible owners of a tile.
 * Some tiles have no owner, so OWNER_NONE is valid.
 */
typedef enum { OWNER_HUMAN, OWNER_AI, OWNER_NONE } Owner;

/*
 * Returns true if the given "cell_type" us a movable unit
 */
int cell_type_is_unit(CellType cell_type);

/*
 * Returns true if the given "cell_type" is a player ownable CellType.
 * All units and the headquarter are player ownable (OWNER_HUMAN or OWNER_AI).
 * The other CellTypes are not player ownable (OWNER_NONE)
 */
int cell_type_is_player_owned(CellType cell_type);

/*
 * This struct represents a grid position.
 * It is used to represent the position of a cell in a level.
 * A position consists of a row and a column ("col").
 * Rows are numbered from top to bottom (starting from 0).
 * Columns are numbered from left to right (starting from 0).
 */
typedef struct
{
    int row;
    int col;
} Pos;

/*
 * This function returns a Pos struct that is initialised with the given row and column ("col").
 * Note that no pointer is returned, so the result is returned using "pass-by-value".
 */
Pos pos_init(int row, int col);

/*
 * This struct represents a Path, which is composed of multiple steps.
 * Each step is a position.
 * The positions are stored in a block of memory to which the "steps" pointer points.
 * The number of positions is stored in "step_count".
 * "distance" stores the distance of the path. This distance takes into account the various distances of each step.
 */
typedef struct
{
    Pos* steps;
    int step_count;
    int distance;
} Path;

/*
 * This function allocates and initialises a Path struct.
 * It allocates and initialises both the Path struct, and the memory that the "steps" pointer points to.
 * The ammount of steps that the "steps" pointer points to is given in "step_count".
 *
 * The function returns a pointer to an allocated and initialised Path.
 */
Path* path_alloc(int step_count, int distance);
/*
 * This function frees the memory to which the "path" pointer points.
 * It also frees the meory that the "steps" pointer points to.
 */
void path_free(Path* path);

#endif
