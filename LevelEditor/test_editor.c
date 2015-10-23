#include "min_unit.h"

#include "common.h"
#include "pqueue.h"
#include "level.h"
#include "pathfinder.h"
#include "dwgraph.h"

#include <stdlib.h>
#include <stdio.h> 

#ifdef _WIN32
#include <process.h>
#endif

static char* test_calculate_cost() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	mu_assert(calculate_cost(&(level->cells[0][0]), &(level->cells[0][1]), (level->cells[0][0]).type) == 12);
	mu_assert(calculate_cost(&(level->cells[0][0]), &(level->cells[1][1]), (level->cells[0][0]).type) == 17);
	mu_assert(calculate_cost(&(level->cells[0][5]), &(level->cells[0][6]), (level->cells[0][5]).type) > 17);
	level_free(level);
	return 0;
}

static char* test_make_graph() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	DWGraph* graph = make_graph(level);
	mu_assert(graph->amountOfColumns == 25);
	mu_assert(graph->amountOfNodes == 25 * 12);
	mu_assert(graph->nodes[0]->cell->type == GROUND);
	mu_assert(graph->nodes[25 + 6]->cell->type == WATER);
	mu_assert(graph->nodes[0]->amountOfNeighbours == 3);
	mu_assert(graph->nodes[25 + 1]->amountOfNeighbours == 8);
	mu_assert(graph->nodes[5]->amountOfNeighbours == 5);
	mu_assert(max(graph->nodes[5]->costs[0], max(graph->nodes[5]->costs[1], max(graph->nodes[5]->costs[2],
		max(graph->nodes[5]->costs[3], graph->nodes[5]->costs[4])))) > 10000);
	mu_assert(min(graph->nodes[5]->costs[0], min(graph->nodes[5]->costs[1], min(graph->nodes[5]->costs[2],
		min(graph->nodes[5]->costs[3], graph->nodes[5]->costs[4])))) == 12);
	free_graph(graph);
	level_free(level);
	return 0;
}


static char* test_cell_to_node() {
	Level* level = level_alloc_empty();
	DWGraph* graph = make_graph(level);
	mu_assert(cell_to_node(graph, &((level->cells)[0][0])) == graph->nodes[0]);
	mu_assert(cell_to_node(graph, &((level->cells)[0][7])) == graph->nodes[7]);
	mu_assert(cell_to_node(graph, &((level->cells)[1][0])) == graph->nodes[25]);
	mu_assert(cell_to_node(graph, &((level->cells)[10][9])) == graph->nodes[25*10+9]);
	free_graph(graph);
	level_free(level);
	return 0;
}


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
	Cell* c0 = &(level->cells)[path->steps[0].row][path->steps[0].col];
	Cell* c1 = &(level->cells)[path->steps[1].row][path->steps[1].col];
	Cell* c2 = &(level->cells)[path->steps[2].row][path->steps[2].col];
	Cell* c3 = &(level->cells)[path->steps[3].row][path->steps[3].col];
	mu_assert(level_can_walk_over(c0, c1));
	mu_assert(level_can_walk_over(c1, c2));
	mu_assert(level_can_walk_over(c2, c3));

	if (path != NULL)
		path_free(path);
	level_free(level);
	return 0;
}

static char* test_path_long_island() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	mu_assert(level != NULL);
	Cell* start_cell = &level->cells[4][6];
	Cell* target_cell = &level->cells[3][18];
	Path* path = find_path(level, start_cell, target_cell);
	mu_assert(path != NULL);
	mu_assert(path->distance == 5 * 17 + 7 * 12);
	mu_assert(path->step_count == 13);
	mu_assert(path->steps != NULL);
	mu_assert(path->steps[0].row == start_cell->row);
	mu_assert(path->steps[0].col == start_cell->col);

	mu_assert(path->steps[1].row == start_cell->row-1);
	mu_assert(path->steps[2].row == start_cell->row-1);
	mu_assert(path->steps[3].row == start_cell->row-1);

	for (int i = 1; i < 12; i++) {	
		mu_assert(path->steps[i].col >= start_cell->col);
	}


	mu_assert(path->steps[path->step_count - 4].row == target_cell->row);
	mu_assert(path->steps[path->step_count - 3].row == target_cell->row);
	mu_assert(path->steps[path->step_count - 2].row == target_cell->row);
	mu_assert(path->steps[path->step_count - 1].row == target_cell->row);

	for (int i = 1; i < 12; i++) {
		mu_assert(path->steps[i].col <= target_cell->col);
	}


	//test if all steps are over terrain that unit in start_cell can walk over
	for (int i = 0; i < 12; i++) {
		Pos* from = &path->steps[i];
		Pos* to = &path->steps[i + 1];
		Cell* cell_from = &level->cells[from->row][from->col];
		Cell* cell_to = &level->cells[to->row][to->col];
		mu_assert(level_can_walk_over(cell_from, cell_to));
	}

	if (path != NULL)
		path_free(path);
	level_free(level);
	return 0;
}

static char* test_path_long_testworld_1() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\used_for_test_1.world");
	mu_assert(level != NULL);
	Cell* start_cell = &level->cells[0][0];
	Cell* target_cell = &level->cells[1][7];
	Path* path = find_path(level, start_cell, target_cell);
	mu_assert(path != NULL);
	mu_assert(path->distance == 9*12+17+12+17+9*12+17+2*12+17+12);
	mu_assert(path->step_count == 10+1+1+1+9+1+2+1+1); /* extra 1, first tile telt ook mee! */
	mu_assert(path->steps != NULL);
	mu_assert(path->steps[0].row == start_cell->row);
	mu_assert(path->steps[0].col == start_cell->col);

	for (int i = 1; i < 11; i++) {
		mu_assert(path->steps[i].row = i);
		mu_assert(path->steps[i].col == 0);
	}
	mu_assert(path->steps[11].row == 11);
	mu_assert(path->steps[11].col == 1);
	mu_assert(path->steps[12].row == 11);
	mu_assert(path->steps[12].col == 2);

	for (int i = 13; i < 23; i++) {
		mu_assert(path->steps[i].row = 11-(i-12));
		mu_assert(path->steps[i].col == 3);
	}
	
	for (int i = 23; i < 26; i++) {
		mu_assert(path->steps[i].row == 0);
		mu_assert(path->steps[i].col == 4+(i-23));
	}


	mu_assert(path->steps[26].row == 1);
	mu_assert(path->steps[26].col == 7);

	mu_assert(path->step_count - 1 == 26);
	mu_assert(path->steps[path->step_count - 1].row == target_cell->row);
	mu_assert(path->steps[path->step_count - 1].col == target_cell->col);


	//test if all steps are over terrain that unit in start_cell can walk over
	for (int i = 0; i < 25; i++) {
		Pos* from = &path->steps[i];
		Pos* to = &path->steps[i + 1];
		Cell* cell_from = &level->cells[from->row][from->col];
		Cell* cell_to = &level->cells[to->row][to->col];
		mu_assert(level_can_walk_over(cell_from, cell_to));
	}

	if (path != NULL)
		path_free(path);
	level_free(level);
	return 0;
}

static char* test_path_long_basic() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\basic.world");
	mu_assert(level != NULL);
	Cell* start_cell = &level->cells[3][2];
	Cell* target_cell = &level->cells[8][22];
	Path* path = find_path(level, start_cell, target_cell);
	mu_assert(path != NULL);
	mu_assert(path->distance == 265);
	mu_assert(path->step_count == 21); /* extra 1, first tile telt ook mee! */

	/* TODO: test afschryven na pathfinder gefixt is.*/

	if (path != NULL)
		path_free(path);
	level_free(level);
	return 0;
}

static char* test_impossible_paths() {
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\used_for_test_1.world");
	mu_assert(level != NULL);
	Cell* start_cell = &level->cells[1][5];
	Cell* target_cell = &level->cells[5][15];
	Path* path = find_path(level, start_cell, target_cell);
	mu_assert(path == NULL);


	level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	mu_assert(level != NULL);
	start_cell = &level->cells[1][5];
	target_cell = &level->cells[11][0];
	path = find_path(level, start_cell, target_cell);
	mu_assert(path == NULL);
	start_cell = &level->cells[1][5];
	target_cell = &level->cells[11][1];
	path = find_path(level, start_cell, target_cell);
	mu_assert(path == NULL);
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

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('*');
	target->owner = level_symbol_to_owner('*');
	mu_assert(level_can_walk_over(unit, target) == 1);

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('B');
	target->owner = level_symbol_to_owner('B');
	mu_assert(level_can_walk_over(unit, target) == 1);

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('W');
	target->owner = level_symbol_to_owner('W');
	mu_assert(level_can_walk_over(unit, target) == 0);

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('R');
	target->owner = level_symbol_to_owner('R');
	mu_assert(level_can_walk_over(unit, target) == 0);

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('3');
	target->owner = level_symbol_to_owner('3');
	mu_assert(level_can_walk_over(unit, target) == 0);

	unit->type = level_symbol_to_cell_type('1');
	unit->owner = level_symbol_to_owner('1');
	target->type = level_symbol_to_cell_type('9');
	target->owner = level_symbol_to_owner('9');
	mu_assert(level_can_walk_over(unit, target) == 0);

	return 0;
}

static char* test_cell_type_is_unit()
{
	mu_assert(!cell_type_is_unit(GROUND));
	mu_assert(!cell_type_is_unit(WATER));
	mu_assert(!cell_type_is_unit(ROCK));
	mu_assert(!cell_type_is_unit(HEADQUARTER));
	mu_assert(!cell_type_is_unit(BRIDGE));
	mu_assert(cell_type_is_unit(UNIT_1));
	mu_assert(cell_type_is_unit(UNIT_2));
	mu_assert(cell_type_is_unit(UNIT_3));
	mu_assert(!cell_type_is_player_owned(DEFAULT_CELLTYPE));
	return 0;
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
	mu_assert(!cell_type_is_player_owned(DEFAULT_CELLTYPE));
    return 0;
}

static char* test_level_symbol_to_cell_type()
{
	mu_assert(level_symbol_to_cell_type('*') == GROUND);
	mu_assert(level_symbol_to_cell_type('W') == WATER);
	mu_assert(level_symbol_to_cell_type('B') == BRIDGE);
	mu_assert(level_symbol_to_cell_type('R') == ROCK);
	mu_assert(level_symbol_to_cell_type('h') == HEADQUARTER);
	mu_assert(level_symbol_to_cell_type('H') == HEADQUARTER);
	mu_assert(level_symbol_to_cell_type('1') == UNIT_1);
	mu_assert(level_symbol_to_cell_type('2') == UNIT_2);
	mu_assert(level_symbol_to_cell_type('3') == UNIT_3);
	mu_assert(level_symbol_to_cell_type('7') == UNIT_1);
	mu_assert(level_symbol_to_cell_type('8') == UNIT_2);
	mu_assert(level_symbol_to_cell_type('9') == UNIT_3);
	mu_assert(level_symbol_to_cell_type('r') == DEFAULT_CELLTYPE);
	mu_assert(level_symbol_to_cell_type(' ') == DEFAULT_CELLTYPE);
	mu_assert(level_symbol_to_cell_type('0') == DEFAULT_CELLTYPE);
	mu_assert(level_symbol_to_cell_type(NULL) == DEFAULT_CELLTYPE);
	mu_assert(level_symbol_to_cell_type(0) == DEFAULT_CELLTYPE);
	mu_assert(level_symbol_to_cell_type("a") == DEFAULT_CELLTYPE);

	return 0;
}

static char* test_level_symbol_to_owner() 
{
	mu_assert(level_symbol_to_owner('*') == OWNER_NONE);
	mu_assert(level_symbol_to_owner('W') == OWNER_NONE);
	mu_assert(level_symbol_to_owner('B') == OWNER_NONE);
	mu_assert(level_symbol_to_owner('R') == OWNER_NONE);
	mu_assert(level_symbol_to_owner('h') == OWNER_HUMAN);
	mu_assert(level_symbol_to_owner('H') == OWNER_AI);
	mu_assert(level_symbol_to_owner('1') == OWNER_HUMAN);
	mu_assert(level_symbol_to_owner('2') == OWNER_HUMAN);
	mu_assert(level_symbol_to_owner('3') == OWNER_HUMAN);
	mu_assert(level_symbol_to_owner('7') == OWNER_AI);
	mu_assert(level_symbol_to_owner('8') == OWNER_AI);
	mu_assert(level_symbol_to_owner('9') == OWNER_AI);
	mu_assert(level_symbol_to_owner('r') == DEFAULT_OWNER);
	mu_assert(level_symbol_to_owner(' ') == DEFAULT_OWNER);
	mu_assert(level_symbol_to_owner('0') == DEFAULT_OWNER);
	mu_assert(level_symbol_to_owner(NULL) == DEFAULT_OWNER);
	mu_assert(level_symbol_to_owner(0) == DEFAULT_OWNER);
	mu_assert(level_symbol_to_owner("a") == DEFAULT_OWNER);

	return 0;
}

static char* test_cell_to_symbol() 
{
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\basic.world");
	mu_assert(cell_to_symbol(&level->cells[0][0]) == '1');
	mu_assert(cell_to_symbol(&level->cells[0][1]) == '2');
	mu_assert(cell_to_symbol(&level->cells[0][2]) == '3');
	mu_assert(cell_to_symbol(&level->cells[3][2]) == 'h');
	mu_assert(cell_to_symbol(&level->cells[9][1]) == 'R');
	mu_assert(cell_to_symbol(&level->cells[11][7]) == 'W');
	mu_assert(cell_to_symbol(&level->cells[11][24]) == '9');
	mu_assert(cell_to_symbol(&level->cells[11][23]) == '8');
	mu_assert(cell_to_symbol(&level->cells[11][22]) == '7');
	mu_assert(cell_to_symbol(&level->cells[9][22]) == 'H');
	mu_assert(cell_to_symbol(&level->cells[5][5]) == '*');
	level_free(level);

	level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	mu_assert(cell_to_symbol(&level->cells[0][0]) == '*');
	mu_assert(cell_to_symbol(&level->cells[11][0]) == '1');
	mu_assert(cell_to_symbol(&level->cells[11][24]) == '7');
	mu_assert(cell_to_symbol(&level->cells[3][8]) == 'B');
	level_free(level);

	return 0;
}

static char* test_cell_type_and_owner_to_symbol() 
{
	mu_assert(cell_type_and_owner_to_symbol(GROUND, OWNER_NONE) == '*');
	mu_assert(cell_type_and_owner_to_symbol(WATER, OWNER_NONE) == 'W');
	mu_assert(cell_type_and_owner_to_symbol(BRIDGE, OWNER_NONE) == 'B');
	mu_assert(cell_type_and_owner_to_symbol(ROCK, OWNER_NONE) == 'R');
	mu_assert(cell_type_and_owner_to_symbol(HEADQUARTER, OWNER_HUMAN) == 'h');
	mu_assert(cell_type_and_owner_to_symbol(HEADQUARTER, OWNER_AI) == 'H');
	mu_assert(cell_type_and_owner_to_symbol(UNIT_1, OWNER_HUMAN) == '1');
	mu_assert(cell_type_and_owner_to_symbol(UNIT_2, OWNER_HUMAN) == '2');
	mu_assert(cell_type_and_owner_to_symbol(UNIT_3, OWNER_HUMAN) == '3');
	mu_assert(cell_type_and_owner_to_symbol(UNIT_1, OWNER_AI) == '7');
	mu_assert(cell_type_and_owner_to_symbol(UNIT_2, OWNER_AI) == '8');
	mu_assert(cell_type_and_owner_to_symbol(UNIT_3, OWNER_AI) == '9');
	mu_assert(cell_type_and_owner_to_symbol(WATER, OWNER_HUMAN) == DEFAULT_SYMBOL);
	mu_assert(cell_type_and_owner_to_symbol(HEADQUARTER, OWNER_NONE) == DEFAULT_SYMBOL);
	mu_assert(cell_type_and_owner_to_symbol(UNIT_2, OWNER_NONE) == DEFAULT_SYMBOL);

	return 0;
}

static char* test_level_is_valid_pos() 
{
	Level* level = level_alloc_read_from_file("..\\assets\\levels\\island.world");
	mu_assert(!level_is_valid_pos(level, -1, -1));
	mu_assert(!level_is_valid_pos(level, -1, 4));
	mu_assert(!level_is_valid_pos(level, 4, -1));
	mu_assert(!level_is_valid_pos(level, 4, 25));
	mu_assert(!level_is_valid_pos(level, 12, 4));
	mu_assert(level_is_valid_pos(level, 11, 23));
	mu_assert(level_is_valid_pos(level, 7, 7));
	mu_assert(level_is_valid_pos(level, 0, 0));
	level_free(level);

	level = level_alloc_empty_with_dim(2, 2);
	mu_assert(!level_is_valid_pos(level, 2, 2));
	mu_assert(!level_is_valid_pos(level, -1, 0));
	mu_assert(!level_is_valid_pos(level, 0, -1));
	mu_assert(level_is_valid_pos(level, 1, 1));
	level_free(level);

	level = level_alloc_empty_with_dim(1, 7);
	mu_assert(!level_is_valid_pos(level, 2, 2));
	mu_assert(!level_is_valid_pos(level, -1, 0));
	mu_assert(!level_is_valid_pos(level, 0, -1));
	mu_assert(level_is_valid_pos(level, 6, 0));
	mu_assert(!level_is_valid_pos(level, 0, 2));
	level_free(level);

	return 0;
}

static char* test_level_alloc_empty_with_dim() {

	Level* level = level_alloc_empty_with_dim(0, 0);
	mu_assert(level->width == LEVEL_MAX_WIDTH && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(-1, -1);
	mu_assert(level->width == LEVEL_MAX_WIDTH && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(10, 10);
	mu_assert(level->width == 10 && level->height == 10);
	level_free(level);

	level = level_alloc_empty_with_dim(100, 100);
	mu_assert(level->width == LEVEL_MAX_WIDTH && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(100, 10);
	mu_assert(level->width == LEVEL_MAX_WIDTH && level->height == 10);
	level_free(level);

	level = level_alloc_empty_with_dim(10, 100);
	mu_assert(level->width == 10 && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(0, 5);
	mu_assert(level->width == LEVEL_MAX_WIDTH && level->height == 5);
	level_free(level);

	level = level_alloc_empty_with_dim(5, 0);
	mu_assert(level->width == 5 && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(-1, 5);
	mu_assert(level->width == LEVEL_MAX_WIDTH && level->height == 5);
	level_free(level);

	level = level_alloc_empty_with_dim(5, -1);
	mu_assert(level->width == 5 && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(24, 11);
	mu_assert(level->width == 24 && level->height == 11);
	level_free(level);

	level = level_alloc_empty_with_dim(1, 1);
	mu_assert(level->width == 1 && level->height == 1);
	level_free(level);

	level = level_alloc_empty_with_dim(11, 24);
	mu_assert(level->width == 11 && level->height == LEVEL_MAX_HEIGHT);
	level_free(level);

	level = level_alloc_empty_with_dim(7, 4);
	mu_assert(level->width == 7 && level->height == 4);
	level_free(level);

	return 0;
}


static char* test_update_pqueue() {
	PriorityQueue* pqueue = pqueue_alloc();

	pqueue_update(pqueue, "abc", 5);
	mu_assert(pqueue->item_count == 1 && pqueue->items[0].cost == 5);
	pqueue_update(pqueue, "def", 7);
	mu_assert(pqueue->item_count == 2 && pqueue->items[1].cost == 7);
	pqueue_update(pqueue, "ghi", 3);
	mu_assert(pqueue->item_count == 3 && pqueue->items[2].cost == 3);
	pqueue_update(pqueue, "ghi", 3);
	mu_assert(pqueue->item_count == 3 && pqueue->items[2].cost == 3);

	pqueue_free(pqueue);
	return 0;
}

static char* test_remove_first_pqueue() {
	PriorityQueue* pqueue = pqueue_alloc();

	pqueue_remove_first(pqueue);
	mu_assert(pqueue->item_count == 0);
	mu_assert(pqueue->items[0].cost == NULL);
	mu_assert(pqueue->items[1].cost == NULL);
	mu_assert(pqueue->items[3].cost == NULL);

	pqueue_update(pqueue, "midden", 5);
	pqueue_update(pqueue, "groot", 7);
	pqueue_update(pqueue, "klein", 3);

	pqueue_remove_first(pqueue);
	mu_assert(pqueue->item_count == 2);
	mu_assert(pqueue->items[0].cost == 5);
	mu_assert(pqueue->items[1].cost == 7);
	mu_assert(pqueue->items[3].cost == NULL);

	pqueue_remove_first(pqueue);
	mu_assert(pqueue->item_count == 1);
	mu_assert(pqueue->items[0].cost == NULL);
	mu_assert(pqueue->items[1].cost == 7);

	pqueue_remove_first(pqueue);
	mu_assert(pqueue->item_count == 0);
	mu_assert(pqueue->items[0].cost == NULL);
	mu_assert(pqueue->items[1].cost == NULL);
	mu_assert(pqueue->items[3].cost == NULL);

	pqueue_free(pqueue);
	return 0;
}

static char* test_level_write_to_file() {

	//level openen en nadien opslaan, vervolgens controleren of alles hetzelfde is gebleven
	Level* level1 = level_alloc_read_from_file("..\\assets\\levels\\basic.world");
	level_write_to_file(level1, "..\\assets\\levels\\basicTest.world");
	Level* level2 = level_alloc_read_from_file("..\\assets\\levels\\basicTest.world");
	mu_assert(level1->height == level2->height && level1->width == level2->width);
	for (int i = 0; i < level1->height; i++) {
		for (int j = 0; j < level1->width; j++) {
			//elke cel moet hetzelfde zijn
			mu_assert(cell_to_symbol(&level1->cells[i][j]) == cell_to_symbol(&level2->cells[i][j]));
		}
	}
	level_free(level1);
	level_free(level2);
	
	level1 = level_alloc_read_from_file("..\\assets\\levels\\2rivers.world");
	level_write_to_file(level1, "..\\assets\\levels\\2riversTest.world");
	level2 = level_alloc_read_from_file("..\\assets\\levels\\2riversTest.world");
	mu_assert(level1->height == level2->height && level1->width == level2->width);
	for (int i = 0; i < level1->height; i++) {
		for (int j = 0; j < level1->width; j++) {
			mu_assert(cell_to_symbol(&level1->cells[i][j]) == cell_to_symbol(&level2->cells[i][j]));
		}
	}
	level_free(level1);
	level_free(level2);

	return 0;
}

static char * all_tests()
{
	//Tests for common.h
	mu_run_test(test_cell_type_is_unit);
	mu_run_test(test_cell_type_is_player_owned);


	//Tests for dwgraph.h
	mu_run_test(test_calculate_cost);
	mu_run_test(test_make_graph);
	mu_run_test(test_cell_to_node);

	//Tests for pathfinder.h
	mu_run_test(test_path_start);
	mu_run_test(test_path_short);
	mu_run_test(test_path_short_island);
	mu_run_test(test_path_long_island);
	mu_run_test(test_path_long_testworld_1);
	mu_run_test(test_impossible_paths);
	mu_run_test(test_path_long_basic);
	//TODO: add more pathfinder tests: longer path, non empty levels, special cases, ...

	//Tests for level.h
	mu_run_test(test_level_symbol_to_cell_type);
	mu_run_test(test_level_symbol_to_owner);
	mu_run_test(test_cell_to_symbol);
	mu_run_test(test_cell_type_and_owner_to_symbol);
	mu_run_test(test_level_can_walk_over);
	mu_run_test(test_level_is_valid_pos);
	mu_run_test(test_level_alloc_empty_with_dim);
	//dit test ook level_alloc_read_from_file
	mu_run_test(test_level_write_to_file);

	//Tests for pqueue.h
	mu_run_test(test_update_pqueue);
	mu_run_test(test_remove_first_pqueue);

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

