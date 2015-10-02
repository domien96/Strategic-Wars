#include "pqueue.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

/*
 * This is an inefficient implementation of a priority queue
 */

PriorityQueue* pqueue_alloc()
{
    int initial_max_items = 10;
    PriorityQueue* res = (PriorityQueue*) calloc(1, sizeof(PriorityQueue));
    res->item_max_count = initial_max_items;
    res->item_count = 0;
    res->items = (QueueItem*) calloc(initial_max_items, sizeof(QueueItem));

    assert(res->items[0].item == NULL);
    assert(res->items[0].cost == 0);
    assert(res->items[initial_max_items-1].item == NULL);
    assert(res->items[initial_max_items-1].cost == 0);

    return res;
}

void pqueue_free(PriorityQueue* queue)
{
    queue->item_max_count = 0;
    queue->item_count = 0;
    free(queue->items);
    free(queue);
}

void pqueue_update(PriorityQueue* q, void* item, int cost)
{
    assert(q != NULL);
    assert(item != NULL);

    int first_empty = -1;
    for (int i = 0; i < q->item_max_count; i++) {
        if (q->items[i].item == item) {
            q->items[i].cost = cost;
            return;
        }
        if (q->items[i].item == NULL && first_empty < 0)
            first_empty = i;
    }
    if (first_empty < 0) {
        int old_size = q->item_max_count;
        int new_size = q->item_max_count * 2;
        q->items = realloc(q->items, new_size * sizeof(QueueItem));
        q->item_max_count = new_size;
        for (int i = old_size; i < new_size; i++) {
            q->items[i].item = NULL;
        }
        first_empty = old_size;
    }
    assert(first_empty >= 0);
    assert(first_empty < q->item_max_count);
    q->items[first_empty].item = item;
    q->items[first_empty].cost = cost;
    q->item_count++;
}

void* pqueue_remove_first(PriorityQueue* q)
{   
    if (q->item_count == 0) {
        return NULL;
    }

    int min_index = -1;
    int min_cost = -1;
    for (int i = 0; i < q->item_max_count; i++) {
        if (q->items[i].item != NULL && (min_cost == -1 || q->items[i].cost < min_cost)) {
            min_cost = q->items[i].cost;
            min_index = i;
        }
    }
    assert(min_index >= 0);
    assert(q->items[min_index].cost == min_cost);
    q->item_count--;
    void* res = q->items[min_index].item;
    q->items[min_index].item = NULL;
    q->items[min_index].cost = 0;
    return res;
}

