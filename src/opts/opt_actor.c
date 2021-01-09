#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "opt_actor.h"
#include "../structs/dictionary.h"
#include "../debug.h"
Options* parse_options(Dictionary* conf, int argc, char** argv) {
	Options* opts = (Options*)malloc(sizeof(Options));
	memset(opts, sizeof(Options), 0);
	opts->dest_host = (char*)dictionary_get(((Dictionary*)dictionary_get(conf, "hosts")), "default");
	int c;
	while (1) {
		c = getopt(argc, argv, "f:h:t:");
		if (c == -1)
			break;
		switch (c) {
			case 'f':
				opts->source_file = optarg;
				break;
			case 'h':
				opts->dest_host = \
				(char*)dictionary_get(((Dictionary*)dictionary_get(conf,"hosts")),\
				optarg);
				break;
			case 't':
				opts->type = optarg;
				break;
			case '?':
				break;
			default:
				fprintf(stderr, "Usage: %s [-f src_file] [-h host_name]", argv[0]);
				break;
		}
	}
	_debug(printf("[D] source file: %s\n", opts->source_file);)
	_debug(printf("[D] destination host: %s\n", opts->dest_host);)
}
void transfer_file(NetWorker* net_worker, Options* opts) {
	FILE *f = fopen(opts->source_file, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
	char *data = malloc(fsize);
	fread(data, 1, fsize, f);
	fclose(f);
	NetMessageOut* msg = (NetMessageOut*)malloc(sizeof(NetMessageOut));
	msg->payload = data;
	msg->payload_size = (uint32_t)fsize;
	msg->port = net_worker->port;
	msg->dest = opts->dest_host;
	queue_insert(net_worker->out_message_queue, msg);
}
void act(NetWorker* net_worker, Options* opts) {
	if (!strcmp(opts->type, "transfer")) {
		transfer_file(net_worker, opts);
	}
}
