#include "common.h"

#include <stdlib.h>

Pos pos_init(int row, int col)
{
	Pos res = {row, col};
    return res;
}

Path* path_alloc(int step_count, int distance)
{
	/*bij steps calloc of malloc gebruiken
	er staat in header dat het geïnitialiseerd moet worden
	maar lijkt mij geen goed idee aangezien (O ,O) 
	een geldige coordinaat is*/
	Pos* steps = (Pos*) malloc(step_count * sizeof(Pos));
	Path* p = (Path*) malloc(sizeof(Path));
	(*p).steps = steps;
	(*p).step_count = step_count;
	(*p).distance = distance;

    return p;
}

/*niet alleen path zelf moet vrijgegeven worden maar ook steps*/
void path_free(Path* path)
{
	free((*path).steps);
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


