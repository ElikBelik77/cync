#ifndef QUEUE_H_
#define QUEUE_H_
#include "dlinkedlist.h"

typedef struct _tagQueue {
	DLinkedList* head;
	DLinkedList* tail;
} Queue;
void  queue_insert(Queue* q, void* element);
void* queue_pop(Queue* q);

#endif
