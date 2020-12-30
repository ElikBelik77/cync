#include <stdio.h>
#include "net/net.h"
#include "structs/dictionary.h"
#include "conf/conf_parser.h"
int test_dict() {
	int blocker;
	NetWorker* worker = init_net_worker(5,5);
	net_worker_free(worker);
	Dictionary* dict = dictionary_init(100);
	Dictionary* nested = dictionary_init(100);
	dictionary_insert(dict, "nested", (void*)nested, (void(*)(void*))dictionary_free);
	dictionary_insert(nested, "hello", (void*)5, NULL);
	printf("%p\n", dictionary_get((Dictionary*)dictionary_get(dict, "nested"), "hello"));
	dictionary_free(dict);
}

int test_conf() {
	Dictionary* conf = parse_conf("./configuration.txt");
	printf("%s\n", (char*)dictionary_get((Dictionary*)dictionary_get(conf, "hosts"),"other_pc"));
//	printf("%s\n", (char*)dictionary_get(conf, "hosts"));
	dictionary_free(conf);
}
int main() {
	test_dict();
	test_conf();
}
