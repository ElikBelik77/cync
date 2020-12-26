#include <assert.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <poll.h>
#include "net.h"
#include "../structs/queue.h"

//Function that guarentee reading/writing of nbytes, otherwise throws an error.
void net_read(int sockfd, size_t nbytes, void* buffer);
void net_write(int sockfd, void* data, size_t nbytes);

// Serializes a netmessage into char* (byte array).
char* net_message_serialize(NetMessage* net_msg) {
        char* sr = malloc(sizeof(net_msg->payload_size) + sizeof(net_msg->status));
        char* ptr = sr;
        memcpy(ptr, &(net_msg->payload_size), sizeof(net_msg->payload_size));
        ptr += sizeof(net_msg->payload_size);
        memcpy(ptr, net_msg->payload, net_msg->payload_size);
        return sr;
}

// Writes a given network message to a socket fd.
void* net_message_write(int sockfd, NetMessage* net_msg) {
	char* serialized_msg = net_message_serialize(net_msg);
	size_t serialized_length = sizeof(net_msg->payload_size) + net_msg->payload_size;
	net_write(sockfd, serialized_msg, serialized_length);
}

// Read a network message from a given socket fd.
NetMessage* net_message_read(int sockfd) {
	// Reads length, then read length bytes.
	size_t length;
	net_read(sockfd, sizeof(uint32_t), &length);
	char* payload_buffer = (char*)malloc(length);
	net_read(sockfd, length, payload_buffer);
	NetMessage* net_msg = (NetMessage*)malloc(sizeof(NetMessage));
	net_msg->payload_size = length;
	net_msg->payload = payload_buffer;
	net_msg->status = Ok;
	return net_msg;
}



int init_net_server(NetWorker* worker) {
	struct sockaddr_in servaddr;
	// Initialzie a tcp socket.
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
	// Setup a pollfd struct for timeout polling.
	struct pollfd pollst;
	memset(&pollst, sizeof(struct pollfd), 0);
	pollst.fd = sockfd;
	pollst.events = POLLIN | POLLPRI;
	while(context->is_running) {
		int retval = poll(&pollst, 1, 2000);
		if (retval == 0) {
			// retval = 0 implies timeout, continues to recheck is_running
			// to halt upon a stop request.
			continue;
		}
		// retval != 0, there is a fd in the pollfd that can be used.
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &address_len);
		NetMessage* cli_msg = net_message_read(connfd);
		queue_insert(context->in_message_queue, cli_msg);
	}
	close(sockfd);
}

// Initializes pthread posix net worker.
pthread_t* init_threading(NetWorker* worker) {
	pthread_t* worker_thread = (pthread_t*)malloc(sizeof(pthread_t));
	assert(worker_thread);
	int result = pthread_create(worker_thread, NULL, net_worker_routine, (void*)worker);
	assert(!result);
	return worker_thread;
}

void net_worker_free(NetWorker* worker) {
		// Ask for stop, upon pollfd timeout the thread will join.
		worker->is_running = 0;
		void* ret_val;
		pthread_join(*(worker->worker_thread), &ret_val);
		queue_free(worker->in_message_queue, NULL);
		queue_free(worker->out_message_queue, NULL);
		free(worker->worker_thread);
		free(worker);
}

void net_read(int sockfd, size_t nbytes, void* buffer) {
        int bytes_read = 0;
        int result;
        while (bytes_read < nbytes) {
                result = read(sockfd, buffer+bytes_read, nbytes-bytes_read);
                if (result < 1) {
                        fprintf(stderr, "error reading net message.\n");
                        exit(0); //TODO: return flag to set status.failed.
                }
                bytes_read += result;
        }

}

void net_write(int sockfd, void* data, size_t nbytes) {
        int bytes_sent = 0;
        int result;
        while(bytes_sent < nbytes) {
                result = write(sockfd, data+bytes_sent, nbytes-bytes_sent);
                if (result == -1) {
                        fprintf(stderr, "error writing to socket.\n");
                        exit(0);
                }
        }
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
	// Initialize threading, the appropriate function appears in the final source
	// according to the OS.
	worker->worker_thread = init_threading(worker);
	return worker;
}


void net_message_free(NetMessage* net_msg) {
        free(net_msg->payload);
        free(net_msg);
}
