#include "min_unit.h"

#include "common.h"
#include "pqueue.h"
#include "level.h"
#include "pathfinder.h"

#include <stdlib.h>
#include <stdio.h> 

#ifdef _WIN32
#include <process.h>
#endif

static char * test_path_start() 
{
    Level* level = level_alloc_empty();
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[3][3];
    Cell* target_cell = start_cell;
    Path* path = find_path(level, start_cell, target_cell);
    mu_assert(path != NULL);
    mu_assert(path->distance == 0);
    mu_assert(path->step_count == 1);
    mu_assert(path->steps != NULL);
    mu_assert(path->steps[0].row == start_cell->row);
    mu_assert(path->steps[0].col == start_cell->col);

    if (path != NULL)
        path_free(path);
    level_free(level);
    return 0;
}

static char * test_path_short() 
{
    Level* level = level_alloc_empty();
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[3][3];
    Cell* target_cell = &level->cells[4][5];
    Path* path = find_path(level, start_cell, target_cell);
    mu_assert(path != NULL);
    mu_assert(path->distance == 12 + 17);
    mu_assert(path->step_count == 3);
    mu_assert(path->steps != NULL);
    mu_assert(path->steps[0].row == start_cell->row);
    mu_assert(path->steps[0].col == start_cell->col);

    mu_assert(path->steps[1].row >= start_cell->row);
    mu_assert(path->steps[1].col >= start_cell->col);
    mu_assert(path->steps[2].row >= start_cell->row);
    mu_assert(path->steps[2].col >= start_cell->col);

    mu_assert(path->steps[1].row <= target_cell->row);
    mu_assert(path->steps[1].col <= target_cell->col);
    mu_assert(path->steps[2].row <= target_cell->row);
    mu_assert(path->steps[2].col <= target_cell->col);

    mu_assert(path->steps[path->step_count-1].row == target_cell->row);
    mu_assert(path->steps[path->step_count-1].col == target_cell->col);

    //TODO: test if all steps are over terrain that unit in start_cell can walk over

    if (path != NULL)
        path_free(path);
    level_free(level);
    return 0;
}

static char* test_path_short_island() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	mu_assert(level != NULL);
	Cell* start_cell = &level->cells[4][6];
	Cell* target_cell = &level->cells[6][9];
	Path* path = find_path(level, start_cell, target_cell);
	mu_assert(path != NULL);
	mu_assert(path->distance == 17 + 17 + 12);
	mu_assert(path->step_count == 4);
	mu_assert(path->steps != NULL);
	mu_assert(path->steps[0].row == start_cell->row);
	mu_assert(path->steps[0].col == start_cell->col);

	mu_assert(path->steps[1].row >= start_cell->row);
	mu_assert(path->steps[1].col >= start_cell->col);
	mu_assert(path->steps[2].row >= start_cell->row);
	mu_assert(path->steps[2].col >= start_cell->col);
	mu_assert(path->steps[3].row >= start_cell->row);
	mu_assert(path->steps[3].col >= start_cell->col);

	mu_assert(path->steps[1].row <= target_cell->row);
	mu_assert(path->steps[1].col <= target_cell->col);
	mu_assert(path->steps[2].row <= target_cell->row);
	mu_assert(path->steps[2].col <= target_cell->col);
	mu_assert(path->steps[3].row <= target_cell->row);
	mu_assert(path->steps[3].col <= target_cell->col);

	mu_assert(path->steps[path->step_count - 1].row == target_cell->row);
	mu_assert(path->steps[path->step_count - 1].col == target_cell->col);

	//test if all steps are over terrain that unit in start_cell can walk over
	Cell* c0 = &path->steps[0];
	Cell* c1 = &path->steps[1];
	Cell* c2 = &path->steps[2];
	Cell* c3 = &path->steps[3];
	mu_assert(level_can_walk_over(c0, c1));
	mu_assert(level_can_walk_over(c1, c2));
	mu_assert(level_can_walk_over(c2, c3));

	if (path != NULL)
		path_free(path);
	level_free(level);
	return 0;
}

static char* test_level_can_walk_over() {
	Cell* unit = & (Cell){0,0,level_symbol_to_cell_type('2'), level_symbol_to_owner('2')};
	Cell* target = &(Cell) { 0, 0, level_symbol_to_cell_type('R'), level_symbol_to_owner('R') };
	mu_assert(level_can_walk_over(unit, target) == 0);

	unit->type = level_symbol_to_cell_type('3');
	unit->owner = level_symbol_to_owner('3');
	target->type = level_symbol_to_cell_type('*');
	target->owner = level_symbol_to_owner('*');
	mu_assert(level_can_walk_over(unit, target) == 1);

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('1');
	target->owner = level_symbol_to_owner('1');
	mu_assert(level_can_walk_over(unit, target) == 0);

	unit->type = level_symbol_to_cell_type('H');
	unit->owner = level_symbol_to_owner('H');
	target->type = level_symbol_to_cell_type('W');
	target->owner = level_symbol_to_owner('W');
	mu_assert(level_can_walk_over(unit, target) == 0);

	unit->type = level_symbol_to_cell_type('h');
	unit->owner = level_symbol_to_owner('h');
	target->type = level_symbol_to_cell_type('B');
	target->owner = level_symbol_to_owner('B');
	mu_assert(level_can_walk_over(unit, target) == 1);

	unit->type = level_symbol_to_cell_type('h');
	unit->owner = level_symbol_to_owner('H');
	target->type = level_symbol_to_cell_type('7');
	target->owner = level_symbol_to_owner('7');
	mu_assert(level_can_walk_over(unit, target) == 1);
}

static char* test_cell_type_is_player_owned()
{
    mu_assert(!cell_type_is_player_owned(GROUND));
    mu_assert(!cell_type_is_player_owned(WATER));
    mu_assert(!cell_type_is_player_owned(ROCK));
    mu_assert(cell_type_is_player_owned(HEADQUARTER));
    mu_assert(!cell_type_is_player_owned(BRIDGE));
    mu_assert(cell_type_is_player_owned(UNIT_1));
    mu_assert(cell_type_is_player_owned(UNIT_2));
    mu_assert(cell_type_is_player_owned(UNIT_3));
    return 0;
}

static char * all_tests() 
{
    //Tests for common.h
    mu_run_test(test_cell_type_is_player_owned);
    //TODO: add other common.h tests
    //mu_run_test(test_cell_type_is_unit);

	mu_run_test(test_path_start);
	mu_run_test(test_path_short);
	mu_run_test(test_path_short_island);
    //Tests for pathfinder.h
    //TODO: add more pathfinder tests: longer path, non empty levels, special cases, ...

    //Tests for level.h
    //TODO
	mu_run_test(test_level_can_walk_over);
    //Tests for dwgraph.h
    //TODO

    //Tests for pqueue.h
    //TODO

    //TODO: add any other tests

    return 0;
}

int main(int argc, char **argv) 
{
    printf("Running tests:\n\n");
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
		printf("\nAT LEAST ONE TEST FAILED\n");
    }
    else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", mu_tests_run);

#ifdef _WIN32
	system("pause");
#endif

    return result != 0;
}

