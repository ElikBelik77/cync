#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdbool.h>
#include "dlinkedlist.h"

typedef struct Queue_t {
	DLinkedList* head;
	DLinkedList* tail;
} Queue;
Queue* queue_create();
void queue_insert(Queue* q, void* element);
void* queue_pop(Queue* q);
bool queue_is_empty(Queue* q);
void queue_free(Queue* q, void (*free_ptr)(void*));
#endif
