#ifndef PQUEUE_H
#define PQUEUE_H

/*
 * This struct represents an item in a PriorityQueue
 */
typedef struct
{
    void* item;
    int cost;
} QueueItem;

/*
 * This struct represents a PriorityQueue
 *
 * A priority queue stores items, and allows:
 *   - adding items (with a certain cost)
 *   - updating a cost for items
 *   - fetching (+ removing) the item with the lowest cost
 */
typedef struct 
{
    QueueItem* items;
    int item_count;
    int item_max_count;
} PriorityQueue;

/*
 * Returns a pointer to a newly initialised PriorityQueue
 */
PriorityQueue* pqueue_alloc();

/*
 * Free the memory used by a PriorityQueue.
 * This does not remove the stored items themselves.
 * (It does remove any other used memory, such as QueueItem)
 */
void pqueue_free(PriorityQueue* queue);

/*
 * Updates the cost associated with a given item in the provided priority_queue.
 *
 * If the item is not yet in the PriorityQueue, it is added.
 */
void pqueue_update(PriorityQueue* q, void* item, int cost);

/*
 * Searches the item with the lowest cost, 
 * removes it from the PriorityQueue, 
 * and returns the item.
 */
void* pqueue_remove_first(PriorityQueue* q);

#endif
