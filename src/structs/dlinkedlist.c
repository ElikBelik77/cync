#include <stdlib.h>
#include <stdio.h>
#include "dlinkedlist.h"
#include "../exit/exit_hooks.h"
DLinkedList* dlist_create() {
	DLinkedList* dlist = (DLinkedList*)malloc(sizeof(DLinkedList));
	if (dlist == NULL) {
		fprintf(stderr, "malloc error.");
//		exit_hook(1);
		exit(1);
	}
	dlist->fd = NULL;
	dlist->bk = NULL;
	dlist->node_data = NULL;
	return dlist;
}

void dlist_free(DLinkedList* dlist, int cascade, void (*node_free)(void*)) {
	DLinkedList* fd = dlist->fd;
	DLinkedList* bk = dlist->bk;
	void* node_data = dlist->node_data;
	if (node_free)
		node_free(node_data);
	free(dlist);
	if (cascade && fd != NULL) {
		dlist_free(fd, 1, node_free);
	}
	if (cascade && bk != NULL) {
		dlist_free(fd, 1, node_free);
	}
}

void dlist_insert(DLinkedList** dlist, void* element) {
	DLinkedList* node = dlist_create();
	node->node_data = element;
	node->bk = *dlist;
	(*dlist)->fd = node;
	*dlist = node;
}

void dlist_unlink(DLinkedList** dlist, void (*node_free)(void*)) {
	if ((*dlist)->fd != NULL) {
		(*dlist)->fd->bk = (*dlist)->bk;
	}
	if ((*dlist)->bk != NULL) {
		(*dlist)->bk->fd = (*dlist)->fd;
	}
	DLinkedList* new_ptr = (*dlist)->fd;
	dlist_free(*dlist, 0, node_free);
	*dlist = new_ptr;
}
