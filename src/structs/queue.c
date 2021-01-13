#include <stdlib.h>
#include <stdio.h>
#include "structs/queue.h"

queue_t* queue_create() {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	dlinkedlist_t* dl = dlist_create();
	queue->head = dl;
	queue->tail = dl;
	return queue;
}

bool queue_is_empty(queue_t* queue) {
	return queue->head == queue->tail;
}

void* queue_pop(queue_t* queue) {
	if (queue_is_empty(queue)) {
		return NULL;
	}
	void* value = queue->tail->bk->node_data;
	// Pop is equivalent to unlinking the front of the underlaying dlist.
	dlinkedlist_t* dl = queue->tail->bk;
	dlist_unlink(&(queue->tail), NULL);
	queue->tail = dl;
	return value;
}

void queue_insert(queue_t* q, void* element) {
	dlist_insert(&(q->head), element);
}

void queue_free(queue_t* queue, void (*free_ptr)(void*)) {
	// free the underlaying dlists and it's elements.
	dlist_free(queue->tail, free_ptr);
	free(queue);
}

