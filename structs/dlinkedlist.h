#ifdef DLINKED_LIST_
#define DLINKED_LIST_
typedef struct _tagDLinkedList {
	struct _tagDLinkedList* fd;
	struct _tagDLinkedList* bk;
	void* node_data;
} DLinkedList;

void dlist_insert(DLinkedList* dlist, void* element);
void dlist_unlink(DLinkedList* dlist);
void dlist_free(DLinkedList* dlist);
#endif

