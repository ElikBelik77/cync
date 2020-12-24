#ifndef NET_H_
#define NET_H_

#include <stdlib.h>
#include "../structs/queue.h"
typedef enum Status { Failed = 0, Ok = 1} Status;
typedef struct NetMessage_t {
	char* message;
	size_t message_size;
	Status status;
} NetMessage;


typedef struct NetWorker_t {
	pid_t pid;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker;


NetWorker* init_net_worker(size_t prot_preamble);
void stop_net_worker(NetWorker* worker);
#endif
