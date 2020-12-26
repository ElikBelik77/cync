#ifndef DLINKED_LIST_
#define DLINKED_LIST_
typedef struct DLinkedList_t {
	struct DLinkedList_t* fd;
	struct DLinkedList_t* bk;
	void* node_data;
} DLinkedList;

/*
	Creates a new doubly linked list.
*/
DLinkedList* dlist_create();

/*
	Frees a doubly linked list.
	node_free: a pointer to a free function that will be used to free node data.
*/
void dlist_free(DLinkedList* dlist, void (*node_free)(void*));

/*
	Inserts a new element at the bk of the given dlist pointer.
	Modifies the given dlist pointer to point to the new element.
	* this function essentially grows the dlist forwards and returns the new head.
*/
void dlist_insert(DLinkedList** dlist, void* element);

/*
	Unlinks a dlist node. 
	The unlink procedue is node->fd->bk = node->bk, node->bk->fd = node->fd
	Modifies the given node pointer to the original node->fd.
*/
void dlist_unlink(DLinkedList** dlist, void (*node_free)(void*));

#endif

