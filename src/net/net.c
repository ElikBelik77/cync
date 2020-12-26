#include <assert.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include "net.h"
#include "../structs/queue.h"
NetMessage* net_message_deserialzie(char* message_data) {
	NetMessage* msg = (NetMessage*)malloc(sizeof(NetMessage));
	memcpy(&(msg->payload_size), message_data, sizeof(msg->payload_size));
	message_data += sizeof(msg->payload_size);
	char* payload_buffer = (char*)malloc(msg->payload_size);
	memcpy(msg->payload, message_data, msg->payload_size);
	memcpy(&(msg->status), message_data + msg->payload_size, sizeof(msg->status));
	return msg;
}
char* net_message_serialize(NetMessage* net_msg) {
	char* sr = malloc(sizeof(net_msg->payload_size) + sizeof(net_msg->status)
					+ net_msg->payload_size);
	char* ptr = sr;
	memcpy(ptr, &(net_msg->payload_size), sizeof(net_msg->payload_size));
	ptr += sizeof(net_msg->payload_size);
	memcpy(ptr, net_msg->payload, net_msg->payload_size);
	ptr += net_msg->payload_size;
	memcpy(ptr, &(net_msg->status), sizeof(net_msg->status));
	return sr;
}

void net_message_free(NetMessage* net_msg) {
	free(net_msg->payload);
	free(net_msg);
}

int init_net_server(NetWorker* worker) {
	struct sockaddr_in servaddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1) {
		fprintf(stderr, "socket creation failed.\n");
		exit(0);
	}
	memset(&servaddr, sizeof(struct sockaddr_in), 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(worker->port);
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		fprintf(stderr, "socket binding failed.\n");
	}
	if ((listen(sockfd, 5)) != 0) {
		fprintf(stderr, "socket listen failed.\n");
	}
	return sockfd;
}

#if defined(unix) || defined(__unix__) || defined(__unix)
#include <pthread.h>
#include <unistd.h>

void* net_worker_routine(void* arg) {
	NetWorker* context = (NetWorker*)arg;
        int connfd, sockfd = init_net_server(context);
	int address_len;
	struct sockaddr_in client_addr;
	struct pollfd pollst;
	memset(&pollst, sizeof(struct pollfd), 0);
	pollst.fd = sockfd;
	pollst.events = POLLIN | POLLPRI;
	while(context->is_running) {
		int retval = poll(&pollst, 1, 2000);
		if (retval == 0) {
			continue;
		}
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &address_len);
		
	}
	close(sockfd);
}

pthread_t* init_threading(NetWorker* worker) {
	pthread_t* worker_thread = (pthread_t*)malloc(sizeof(pthread_t));
	assert(worker_thread);
	int result = pthread_create(worker_thread, NULL, net_worker_routine, (void*)worker);
	assert(!result);
	return worker_thread;
}

void net_worker_free(NetWorker* worker) {
		worker->is_running = 0;
		void* ret_val;
		pthread_join(*(worker->worker_thread), &ret_val);
		queue_free(worker->in_message_queue, NULL);
		queue_free(worker->out_message_queue, NULL);
		free(worker->worker_thread);
		free(worker);
}

#endif

#if defined(__WIN32)
// Define windows threading initialization.
#endif

NetWorker* init_net_worker(int port, size_t prot_preamble) {
	NetWorker* worker = (NetWorker*)malloc(sizeof(NetWorker));
	assert(worker);
	worker->is_running = 1;
	worker->in_message_queue  = queue_create();
	worker->out_message_queue = queue_create();
	worker->worker_thread = init_threading(worker);
	return worker;
}
