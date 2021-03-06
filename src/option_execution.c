#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "option_execution.h"

options_t *parse_options(dictionary_t *conf, int argc, char **argv) {
	options_t *opts = (options_t*)malloc(sizeof(options_t));
	memset(opts, 0, sizeof(options_t));
	opts->dest_host = (char *)dictionary_get(
	((dictionary_t *)dictionary_get(conf, "hosts")), "default");
	int c;
	while (1) {
		c = getopt(argc, argv, "f:h:t");
		if (c == -1)
			break;
		switch (c) {
			case 'f':
				opts->source_file = optarg;
				break;
			case 'h':
				opts->dest_host = (char *)dictionary_get(
				((dictionary_t *)dictionary_get(conf, "hosts")), optarg);
				break;
			case 't':
				opts->transfer = 1;
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
	return opts;
}

void transfer_file(net_worker_t *net_worker, options_t *opts) {
	FILE *f = fopen(opts->source_file, "rb");
	if (f == NULL) {
		printf("error\n");
		fflush(stdout);
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */
	char *data = malloc(fsize);
	fread(data, 1, fsize, f);
	fclose(f);
	net_message_out_t *msg = (net_message_out_t*)malloc(sizeof(net_message_out_t));
	msg->payload = data;
	msg->payload_size = (uint32_t)fsize;
	msg->port = net_worker->port;
	msg->dest = opts->dest_host;
	queue_insert(net_worker->out_message_queue, msg);
}
void act(net_worker_t *net_worker, options_t* opts) {
  if (opts->transfer) {
    transfer_file(net_worker, opts);
  }
}
