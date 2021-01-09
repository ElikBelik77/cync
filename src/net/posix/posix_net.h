#ifdef UNIX
#ifndef POSIX_NET_H_
#define POSIX_NET_H_

#include <pthread.h>

#include "../../structs/queue.h"

/*
	Struct for NetWorker objects.
*/
typedef struct NetWorker_t {
	int sock_fd, port;
	volatile bool is_running;
	pthread_t* worker_thread;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker;

void init_threading(NetWorker* worker);

#endif
#endif
