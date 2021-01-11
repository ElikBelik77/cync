#pragma once
#ifndef NET_H_
#include <stdbool.h>
#include <stdint.h>

#include "net/posix/posix_net.h"
#include "net/win32/win32_net.h"
#define NET_H_

/*
	Enum for message statuses.
*/
typedef enum Status_e { Failed = 0, Ok = 1} Status;


/*
	Struct for describing inbound network messages. It is packed for compiler-independent parsing.
*/
typedef struct __attribute__((__packed__)) NetMessageIn_t {
	uint32_t payload_size;
	char* payload;
} NetMessageIn;


/*
	Strcut for outbound network messages.
*/
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
void net_message_out_free(NetMessageOut* mess0age);

/*
	Serializes a network message.
*/
char* net_message_serialize(NetMessageOut* message);
/*
	Initializes a new networker based on the operating system.
*/
NetWorker* init_net_worker(int port);


/*
	Frees a networker.
*/
void net_worker_free(NetWorker* worker);


#endif
