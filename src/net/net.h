#ifndef NET_H_
#define NET_H_

#include <stdlib.h>
#include <stdbool.h>
#include "../structs/queue.h"
typedef enum Status { Failed = 0, Ok = 1} Status;
typedef struct NetMessage_t {
	char* message;
	size_t message_size;
	Status status;
} NetMessage;

#if defined(unix) || defined(__unix__) || defined(__unix)
#include <pthread.h>
typedef struct NetWorker_t {
	int sock_fd, port;
	volatile bool is_running;
	size_t prot_preamble;
	pthread_t* worker_thread;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker;
#endif

#if defined(__WIN32)
typedef struct NetWorker_t {
	Queue* in_message_queue;
	Queue* out_message_queue;
}
#endif

NetWorker* init_net_worker(int port, size_t prot_preamble);
void stop_net_worker(NetWorker* worker);
void net_worker_free(NetWorker* worker);
#endif
