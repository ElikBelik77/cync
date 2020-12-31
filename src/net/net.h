#ifndef NET_H_
#define NET_H_
#define NET_PORT 1234
#include <stdbool.h>
#include <stdint.h>
#include <netinet/in.h>
#include "../structs/queue.h"
/*
	Enum for message statuses.
*/
typedef enum Status_e { Failed = 0, Ok = 1} Status;


/*
	Struct for describing network messages. It is packed for compiler-independent parsing.
*/
typedef struct __attribute__((__packed__)) NetMessageIn_t {
	uint32_t payload_size;
	char* payload;
} NetMessageIn;

typedef struct __attribute__((__packed__)) NetMessageOut_t {
	uint32_t payload_size;
	char* payload;
	char* dest;
	short port;
} NetMessageOut;

/*
	Frees a network message.
*/
void net_message_in_free(NetMessageIn* message);
void net_message_out_free(NetMessageOut* message);

#if defined(unix) || defined(__unix__) || defined(__unix)
#include <pthread.h>
/*
	Unix definition of network worker.
	Holds all the data needed for a background networking pthread.
*/
typedef struct NetWorker_t {
	int sock_fd, port;
	volatile bool is_running;
	pthread_t* worker_thread;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker;
#endif

#if defined(__WIN32)
/*
	Windows definition of network worker.
*/
typedef struct NetWorker_t {
	Queue* in_message_queue;
	Queue* out_message_queue;
}
#endif

/*
	Initializes a new networker based on the operating system.
*/
NetWorker* init_net_worker(int port);


/*
	Stops a networker.
*/
void stop_net_worker(NetWorker* worker);


/*
	Frees a networker.
*/
void net_worker_free(NetWorker* worker);
#endif
