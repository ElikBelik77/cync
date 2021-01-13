#pragma once
#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdbool.h>
#include "dlinkedlist.h"

typedef struct {
	dlinkedlist_t* head;
	dlinkedlist_t* tail;
} queue_t;

/*
	Creates a new queue.
*/
queue_t* queue_create();

/*
	Inserts a new element to the back of the queue.
*/
void queue_insert(queue_t* q, void* element);

/*
	Pops an element from the front of the queue.
*/
void* queue_pop(queue_t* q);

/*
	Checks if a given queue is empty.
*/
bool queue_is_empty(queue_t* q);

/*
	Frees a queue
	free_ptr: a function pointer that will be used to free the data
		  at every queue node.
*/
void queue_free(queue_t* q, void (*free_ptr)(void*));
#endif
