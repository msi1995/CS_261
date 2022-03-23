/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>

#include "dynarray.h"
#include "pq.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 * in addition, you want to define an element struct with both data and priority, 
 * corresponding to the elements of the priority queue. 
 */
struct pq {
	struct dynarray* values;
	struct dynarray* prio;
};


void swap(struct pq* pq, int node1, int node2) {
	void* temp_value = dynarray_get(pq->values, node1);
	void* temp_prio = dynarray_get(pq->prio, node1);

	dynarray_set(pq->values, node1, dynarray_get(pq->values, node2));
	dynarray_set(pq->prio, node1, dynarray_get(pq->prio, node2));
	dynarray_set(pq->values, node2, temp_value);
	dynarray_set(pq->prio, node2, temp_prio);

}

void perc_up(struct pq* pq, int end_pos) {
	if (end_pos != 0) {
		int parent = ((end_pos - 1) / 2);

		int end_prio = (*((int*)dynarray_get(pq->prio, end_pos)));
		int parent_prio = (*((int*)dynarray_get(pq->prio, parent)));

		if (end_prio > parent_prio) {
			swap(pq, parent, end_pos);
			perc_up(pq, parent);
		}
	}
}

void perc_down(struct pq* pq, int end_pos) {

	int max;
	int left_child = end_pos * 2 + 1;
	int right_child = end_pos * 2 + 2;

	if (right_child >= dynarray_length(pq->values)) {
		if (left_child >= dynarray_length(pq->values)) {
			return;
		}
		else max = left_child;
	}
	else {
		int left_child_prio = *((int*)dynarray_get(pq->prio, left_child));
		int right_child_prio = *((int*)dynarray_get(pq->prio, right_child));
		if (left_child_prio >= right_child_prio) {
			max = left_child;
		}
		else {
			max = right_child;
		}
	}

	if ((*((int*)dynarray_get(pq->prio, end_pos))) < (*((int*)dynarray_get(pq->prio, max)))) {
		swap(pq, end_pos, max);

		perc_down(pq, max);
	}
}

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* prio_queue = malloc(sizeof(struct pq));

	prio_queue->values = dynarray_create();
	prio_queue->prio = dynarray_create();

	return prio_queue;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	dynarray_free(pq->prio);
	dynarray_free(pq->values);

	free(pq);

}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if(dynarray_length(pq->values) == 0)
		return 1;
	else
		return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, higher priority
 * values are given precedent, and higher place in the queue.  In other words, the
 * element in the priority queue with the highest priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   data - the data value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, higher priority values
 *     should correspond to the first elements.  In other words,
 *     the element in the priority queue with the highest priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* data, int priority) {
	
	int end_pos = dynarray_length(pq->values);

	int* prio = malloc(4);
	*prio = priority;

	dynarray_insert(pq->values, end_pos, data);
	dynarray_insert(pq->prio, end_pos, prio);

	perc_up(pq, end_pos);

}


/*
 * This function should return the data of the first element in a priority
 * queue, i.e. the data associated with the element with highest priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the data of the first item in pq, i.e. the item with
 *   max priority value.
 */
void* pq_max(struct pq* pq) {

	return dynarray_get(pq->values, 0);
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with highest priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with highest priority value.
 */
int pq_max_priority(struct pq* pq) {
	int maxprio = *((int*) dynarray_get(pq->prio, 0));

	return maxprio;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with highest priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   highest priority value.
 */
void* pq_max_dequeue(struct pq* pq) {

	void* value = dynarray_get(pq->values, 0);
	int tail = dynarray_length(pq->values) - 1;

	swap(pq, 0, tail);

	dynarray_remove(pq->values, tail);
	dynarray_remove(pq->prio, tail);

	if(dynarray_length(pq->values) > 0)
	perc_down(pq, 0);

	return value;
}
