#ifndef NET_H_
#include "../structs/queue.h";
#define NET_H_
typedef enum Status { Failed = 0, Ok = 1} Status;
typedef struct NetMessage_t {
	char* message;
	size_t message_size;
	Status status;
} NetMessage


typedef struct NetWorker_t {
	pid_t pid;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker


NetWorker* init_net_worker(size_t prot_preamble);

#endif
