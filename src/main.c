#include <stdio.h>
#include "net/net.h"
#include "structs/dictionary.h"
int main() {
	int blocker;
	NetWorker* worker = init_net_worker(5,5);
	net_worker_free(worker);
	Dictionary* dict = dictionary_init(100);
	printf("dict init\n");
	dictionary_insert(dict, "hello", (void*)5, NULL);
	printf("dict insert\n");
	printf("hello: %p", dictionary_get(dict, "hello"));
	printf("hello: %p", dictionary_get(dict, "hello"));
	printf("dict get\n");
	dictionary_free(dict);
}

