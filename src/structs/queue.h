#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdbool.h>
#include "dlinkedlist.h"

typedef struct Queue_t {
	DLinkedList* head;
	DLinkedList* tail;
} Queue;

/*
	Creates a new queue.
*/
Queue* queue_create();

/*
	Inserts a new element to the back of the queue.
*/
void queue_insert(Queue* q, void* element);

/*
	Pops an element from the front of the queue.
*/
void* queue_pop(Queue* q);

/*
	Checks if a given queue is empty.
*/
bool queue_is_empty(Queue* q);

/*
	Frees a queue
	free_ptr: a function pointer that will be used to free the data
		  at every queue node.
*/
void queue_free(Queue* q, void (*free_ptr)(void*));
#endif
