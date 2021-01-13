#pragma once
#ifndef NET_H_
#include <stdbool.h>
#include <stdint.h>

#include "net/posix/posix_net.h"
#include "net/win32/win32_net.h"
#define NET_H_

/*
	Struct for describing inbound network messages. It is packed for compiler-independent parsing.
*/
typedef struct __attribute__((__packed__)) {
	uint32_t payload_size;
	char* payload;
} net_message_in_t;


/*
	Strcut for outbound network messages.
*/
typedef struct __attribute__((__packed__)) {
	uint32_t payload_size;
	char* payload;
	char* dest;
	short port;
} net_message_out_t;

/*
	Frees a network message.
*/
void net_message_in_free(net_message_in_t* message);
void net_message_out_free(net_message_out_t* message);

/*
	Serializes a network message.
*/
char* net_message_serialize(net_message_out_t *message);
/*
	Initializes a new networker based on the operating system.
*/
net_worker_t* init_net_worker(int port);


/*
	Frees a networker.
*/
void net_worker_free(net_worker_t* worker);


#endif
