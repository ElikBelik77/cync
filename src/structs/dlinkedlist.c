#include <stdlib.h>
#include <stdio.h>
#include "structs/dlinkedlist.h"

dlinkedlist_t* dlist_create() {
	dlinkedlist_t* dlist = (dlinkedlist_t*)malloc(sizeof(dlinkedlist_t));
	if (dlist == NULL) {
		fprintf(stderr, "malloc error.");
		exit(1);
	}
	dlist->fd = NULL;
	dlist->bk = NULL;
	dlist->node_data = NULL;
	return dlist;
}

void dlist_insert(dlinkedlist_t** dlist, void* element) {
	dlinkedlist_t* node = dlist_create();
	node->node_data = element;
	node->fd = *dlist;
	(*dlist)->bk = node;
	*dlist = node;
}

void dlist_unlink(dlinkedlist_t** dlist, void (*node_free)(void*)) {
	if ((*dlist)->fd != NULL) {
		(*dlist)->fd->bk = (*dlist)->bk;
	}
	if ((*dlist)->bk != NULL) {
		(*dlist)->bk->fd = (*dlist)->fd;
	}
	dlinkedlist_t* new_ptr = (*dlist)->fd;
	if (node_free) {
		node_free((*dlist)->node_data);
	}
	free(*dlist);
	*dlist = new_ptr;
}

void dlist_free(dlinkedlist_t* dlist, void (*node_free)(void*)) {
	//To free a dlist basically recursively unlink every node until there's
	// nothing left.
        dlinkedlist_t* fd = dlist->fd;
        dlinkedlist_t* bk = dlist->bk;
        dlist_unlink(&dlist, node_free);
	if (fd)
		dlist_free(fd, node_free);
	if (bk)
		dlist_free(bk, node_free);
}
