#pragma once
#ifdef UNIX
#ifndef POSIX_NET_H_
#define POSIX_NET_H_

#include <pthread.h>

#include "structs/queue.h"

/*
	Struct for NetWorker objects.
*/
typedef struct {
	int sock_fd, port;
	volatile bool is_running;
	pthread_t *worker_thread;
	queue_t *in_message_queue;
	queue_t *out_message_queue;
} net_worker_t;

void init_threading(net_worker_t *worker);

#endif
#endif


typedef int silence_iso_posix_net;
