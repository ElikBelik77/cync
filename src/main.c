#include <stdio.h>
#include "net/net.h"
#include "structs/dictionary.h"
#include "conf/conf_parser.h"
int test_dict() {
	Dictionary* dict = dictionary_init(100);
	Dictionary* nested = dictionary_init(100);
	dictionary_insert(dict, "nested", (void*)nested, (void(*)(void*))dictionary_free);
	dictionary_insert(nested, "hello", (void*)5, NULL);
	printf("%p\n", dictionary_get((Dictionary*)dictionary_get(dict, "nested"), "hello"));
	dictionary_free(dict);
}

int test_conf() {
	Dictionary* conf = parse_conf("./configuration.txt");
	printf("%p\n", (char*)dictionary_get((Dictionary*)dictionary_get(conf, "hosts"),"other_p"));
//	printf("%s\n", (char*)dictionary_get(conf, "hosts"));
	dictionary_free(conf);
}
#ifdef TEST
int main(int argc, char** argv) {
	printf("[1] testing dictionaries\n");
	test_dict();
	printf("[1] test done\n");
	printf("[2] testing configuration parser\n");
	test_conf();
	printf("[2] test done\n");
}
#endif
#ifndef TEST
int main(int argc, char** argv) {
	Dictionary* conf = parse_conf("./configuration.txt");
	#ifdef DEBUG
	printf("Configuration:\n");
	dictionary_print(conf);
	#endif
	char* s_port;
	if ((s_port = dictionary_get(conf, "lport")) == NULL) {
		fprintf(stderr, "Invalid configuration setup.\n");
		exit(0);
	}
	NetWorker* worker = init_net_worker(atoi(s_port));
	net_worker_free(worker);
}
#endif
