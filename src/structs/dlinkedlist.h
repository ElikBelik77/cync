#ifndef DLINKED_LIST_
#define DLINKED_LIST_
typedef struct DLinkedList_t {
	struct DLinkedList_t* fd;
	struct DLinkedList_t* bk;
	void* node_data;
} DLinkedList;
DLinkedList* dlist_create();
void dlist_free(DLinkedList* dlist, int cascade, void (*node_free)(void*));
void dlist_insert(DLinkedList** dlist, void* element);
void dlist_unlink(DLinkedList** dlist, void (*node_free)(void*));

#endif

