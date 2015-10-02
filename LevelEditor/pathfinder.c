#include "pathfinder.h"

#include "dwgraph.h"
#include "pqueue.h"
#include "level.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>


Path* find_path(Level* level, Cell* start, Cell* target)
{
    //this is a dummy implementation, and is to be replaced by a real implementation
    Path* res = path_alloc(100, 0);

    Pos cur;
    cur.row = start->row;
    cur.col = start->col;

    res->step_count = 1;
    res->steps[0] = cur;

    while (cur.row != target->row || cur.col != target->col)
    {
        if (cur.row < target->row) cur.row += 1;
        if (cur.col < target->col) cur.col += 1;
        if (cur.row > target->row) cur.row -= 1;
        if (cur.col > target->col) cur.col -= 1;

        if (res->step_count >= 100)
        {
            free(res->steps);
            free(res);
            return NULL;
        }

        res->steps[res->step_count] = cur;
        res->step_count++;
        res->distance += 12;
    }

    return res;
}

