#include "common.h"

#include <stdlib.h>

Pos pos_init(int row, int col)
{
    Pos res;
    return res;
}
/*commit test*/
Path* path_alloc(int step_count, int distance)
{
	Pos* steps = (Pos*)calloc(step_count, sizeof(Pos));
	Path* path = (Path*) calloc(1,sizeof(Path));
	(*path).steps = steps;
	(*path).distance = distance;
	(*path).step_count = step_count;
    return path;
}

void path_free(Path* path)
{
	free(path->steps);
	free(path);
}


int cell_type_is_unit(CellType cell_type)
{
    return cell_type == UNIT_1 || cell_type == UNIT_2 || cell_type == UNIT_3;
}

int cell_type_is_player_owned(CellType cell_type)
{
	return cell_type_is_unit(cell_type) || cell_type == HEADQUARTER;
}

