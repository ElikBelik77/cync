#ifndef NET_H_
#define NET_H_

#include <stdbool.h>
#include <stdint.h>
#include "../structs/queue.h"
typedef enum Status_e { Failed = 0, Ok = 1} Status;
typedef struct __attribute__((__packed__)) NetMessage_t {
	uint32_t payload_size;
	char* payload;
	Status status;
} NetMessage;
void net_message_free(NetMessage* message);
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
