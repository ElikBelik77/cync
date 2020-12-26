#include <stdlib.h>
#include "queue.h"
#include "dlinkedlist.h"
Queue* queue_create() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	DLinkedList* dl = dlist_create();
	queue->head = dl;
	queue->tail = dl;
}

bool queue_is_empty(Queue* queue) {
	return queue->head == queue->tail;
}

void* queue_pop(Queue* queue) {
	if (queue_is_empty(queue)) {
		return NULL;
	}
	void* value = queue->tail->fd->node_data;
	// Pop is equivalent to unlinking the front of the underlaying dlist.
	dlist_unlink(&(queue->tail), NULL);
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

