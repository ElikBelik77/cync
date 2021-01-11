#include <stdlib.h>
#include <stdio.h>
#include "structs/queue.h"

Queue* queue_create() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	DLinkedList* dl = dlist_create();
	queue->head = dl;
	queue->tail = dl;
	return queue;
}

bool queue_is_empty(Queue* queue) {
	return queue->head == queue->tail;
}

void* queue_pop(Queue* queue) {
	if (queue_is_empty(queue)) {
		return NULL;
	}
	void* value = queue->tail->bk->node_data;
	// Pop is equivalent to unlinking the front of the underlaying dlist.
	DLinkedList* dl = queue->tail->bk;
	dlist_unlink(&(queue->tail), NULL);
	queue->tail = dl;
	return value;
}

void queue_insert(Queue* q, void* element) {
	dlist_insert(&(q->head), element);
}

void queue_free(Queue* queue, void (*free_ptr)(void*)) {
	// free the underlaying dlists and it's elements.
	dlist_free(queue->tail, free_ptr);
	free(queue);
}

