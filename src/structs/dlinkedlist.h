#ifndef DLINKED_LIST_
#define DLINKED_LIST_
typedef struct _tagDLinkedList {
	struct _tagDLinkedList* fd;
	struct _tagDLinkedList* bk;
	void* node_data;
} DLinkedList;
DLinkedList* dlist_create();
void dlist_free(DLinkedList* dlist, int cascade, void (*node_free)(void*));
void dlist_insert(DLinkedList** dlist, void* element);
void dlist_unlink(DLinkedList** dlist, void (*node_free)(void*));

#endif

