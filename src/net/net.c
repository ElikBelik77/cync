#include "net.h"
#include "../structs/queue.h"

#include <pthread.h>

NetWorker* init_net_worker(size_t prot_preamble) {
	NetWorker* worker = (NetWorker*)malloc(sizeof(NetWorker));
	worker->in_message_queue  = queue_create();
	worker->out_message_queue = queue_create();
	return worker;
}



void stop_net_worker(NetWorker* worker) {
	queue_free(worker->in_message_queue, NULL);
	queue_free(worker->out_message_queue, NULL);
}
