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
int main(int argc, char** argv) {
//	test_dict();
//	test_conf();
	Dictionary* conf = parse_conf("./configuration.txt");
	char* s_port;
	if ((s_port = dictionary_get(conf, "lport")) == NULL) {
		fprintf(stderr, "Invalid configuration setup.\n");
		exit(0);
	}
	NetWorker* worker = init_net_worker(atoi(s_port));
}
