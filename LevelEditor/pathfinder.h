#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "level.h"

/*
 * This function allocates a Path struct, and returns a pointer to it.
 * The path is initialised and contains the steps needed
 * to walk the shortest path between the "start" and "target"
 * Cells in the given "level".
 *
 * If no path is possible, NULL is returned instead.
 *
 * Implementation details:
 * This function must be implemented by converting the level into a DWGraph, 
 * and must then search the shortest path between the start and target cell 
 * in that graph. Finally, it should convert the result to a Path struct and 
 * return it.
 * Note that this function will probably use helper functions for this,
 * which should be declared in this header.
 * */
Path* find_path(Level* level, Cell* start, Cell* target);

#endif

