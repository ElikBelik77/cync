#include "net/net.h"
//net must come before stdio.h for windows compactability.
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "structs/dictionary.h"
#include "conf/conf_parser.h"
#include "debug.h"

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
	dictionary_print(conf);
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
typedef struct Options_t {
	char* src_file;
	char* dest_host;
} Options;
Options* parse_options(Dictionary* conf, int argc, char** argv) {
	char* src_file = NULL;
	char* dest_host = (char*)dictionary_get(((Dictionary*)dictionary_get(conf, "hosts")),"default");
	int c;
	while (1) {
		c = getopt(argc, argv, "f:h:");
		/* Detect the end of the options. */
		if (c == -1)
			break;
		switch (c) {
			case 'f':
				src_file = optarg;
				break;

			case 'h':
				dest_host = \
				(char*)dictionary_get(((Dictionary*)\
				dictionary_get(conf, "hosts")),\
				optarg);
			case '?':
				break;
		        default:
				fprintf(stderr, "Usage: %s"\
				" [-f/--file file_path]"\
				" [-h/--host host_name]", argv[0]);
				exit(0);
		}
	}
	Options* opts = (Options*)malloc(sizeof(Options));
	opts->src_file = src_file;
	opts->dest_host = dest_host;
	_debug(printf("[D] source file: %s\n", opts->src_file);)
	_debug(printf("[D] destination host: %s\n", opts->dest_host);)
	return opts;
}
void send_options(NetWorker* worker, Options* opts) {
	NetMessageOut* msg = (NetMessageOut*)malloc(sizeof(NetMessageOut));
	msg->payload = strdup(opts->src_file);
	msg->payload_size = strlen(opts->src_file);
	msg->port = worker->port;
	msg->dest = opts->dest_host;
	queue_insert(worker->out_message_queue, msg);
	char b;
	scanf("%c",&b);
}
int main(int argc, char** argv) {
	Dictionary* conf = parse_conf("./configuration.txt");
	_debug(printf("Configuration:\n");)
	_debug(dictionary_print(conf);)
	char* s_port;
	if ((s_port = dictionary_get(conf, "lport")) == NULL) {
		fprintf(stderr, "Invalid configuration setup.\n");
		exit(0);
	}
	NetWorker* worker = init_net_worker(atoi(s_port));
	Options* opts = parse_options(conf, argc, argv);
	send_options(worker, opts);
	net_worker_free(worker);
	dictionary_free(conf);
	free(opts);
}

#endif

