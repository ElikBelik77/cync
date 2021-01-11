#ifdef UNIX
#include <assert.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <poll.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "net/posix/posix_net.h"
#include "net/net.h"
#include "structs/queue.h"
#include "common.h"

void net_read(int sockfd, uint32_t nbytes, char* buffer);
void net_write(int sockfd, uint32_t nbytes, char* buffer);

// Writes a given network message to a socket fd.
void net_message_write(NetMessageOut* net_msg) {
	struct sockaddr_in dest_addr;
	int sockfd;
	memset(&dest_addr, 0, sizeof(struct sockaddr_in));
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		fprintf(stderr, "socket open error.\n");
		return;
	}
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(net_msg->port);
	dest_addr.sin_addr.s_addr = inet_addr(net_msg->dest);
	_debug(printf("[D] connecting to %s\n", net_msg->dest);)
	if (connect(sockfd, (struct sockaddr *)&dest_addr, \
					sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "socket connection error.\n");
		return;
	}
	char* serialized_msg = net_message_serialize(net_msg);
	uint32_t serialized_length = sizeof(uint32_t) + net_msg->payload_size;
	net_write(sockfd, serialized_length, serialized_msg);
	close(sockfd);
}

// Read a network message from a given socket fd.
NetMessageIn* net_message_read(int sockfd) {
	// Reads length, then read length bytes.
	uint32_t length;
	net_read(sockfd, sizeof(uint32_t), (char*)&length);
	char* payload_buffer = (char*)malloc(length+1);
	net_read(sockfd, length, payload_buffer);
	NetMessageIn* net_msg = (NetMessageIn*)malloc(sizeof(NetMessageIn));
	net_msg->payload_size = length;
	net_msg->payload = payload_buffer;
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
	worker->sock_fd = sockfd;
	return sockfd;
}

void* net_sender_routine(void* arg) {
	NetWorker* context = (NetWorker*)arg;
	while (context->is_running) {
		sleep(1);
		while (!queue_is_empty(context->out_message_queue)) {
			NetMessageOut* msg = (NetMessageOut*)queue_pop(context->out_message_queue);
			_debug(printf("[D] Popped a message to send to %s\n", msg->dest);)
			_debug(fflush(stdout);)
			net_message_write(msg);
			net_message_out_free(msg);
		}
	}
	return NULL;
}

void* net_worker_routine(void* arg) {
	NetWorker* context = (NetWorker*)arg;
	int connfd, sockfd = init_net_server(context);
	pthread_t* sender_thread = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(sender_thread, NULL, net_sender_routine, (void*)context);
	unsigned int address_len = 0;
	struct sockaddr_in client_addr;
	// Setup a pollfd struct for timeout polling.
	struct pollfd pollst;
	memset(&pollst, 0, sizeof(struct pollfd));
	pollst.fd = sockfd;
	pollst.events = POLLIN | POLLPRI;
	while(context->is_running) {
		int retval = poll(&pollst, 1, 2000);
		if (retval == 0) {
			continue;
			// retval = 0 implies timeout, continues to recheck is_running
			// to halt upon a stop request.
		}
		address_len = 0;
		memset(&client_addr, 0, sizeof(client_addr));
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &address_len);
		NetMessageIn* cli_msg = net_message_read(connfd);
		queue_insert(context->in_message_queue, cli_msg);
		_debug(printf("[D] Message received size: %d\n",cli_msg->payload_size);)
	}
	void* ret_val;
	pthread_join(*(sender_thread), &ret_val);
	_debug(printf("[D] Sender thread joined.\n");)
	free(sender_thread);
	close(sockfd);
	return NULL;
}

void init_threading(NetWorker* worker) {
	pthread_t* worker_thread = (pthread_t*)malloc(sizeof(pthread_t));
	assert(worker_thread);
	int result = pthread_create(worker_thread, NULL, net_worker_routine, (void*)worker);
	assert(!result);
	worker->worker_thread = worker_thread;
}

void net_worker_free(NetWorker* worker) {
		// Ask for stop, upon pollfd timeout the thread will join.
		worker->is_running = 0;
		void* ret_val;
		pthread_join(*(worker->worker_thread), &ret_val);
		_debug(printf("[D] Net thread joined.\n");)
		queue_free(worker->in_message_queue, NULL);
		queue_free(worker->out_message_queue, NULL);
		free(worker->worker_thread);
		free(worker);
}

void net_read(int sockfd, uint32_t nbytes, char* buffer) {
        uint32_t bytes_read = 0;
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

void net_write(int sockfd, uint32_t nbytes, char* data) {
        uint32_t bytes_sent = 0;
        int result;
        while(bytes_sent < nbytes) {
                result = write(sockfd, data+bytes_sent, nbytes-bytes_sent);
		if(result == -1) {
                        fprintf(stderr, "error writing to socket.\n");
                        exit(0);
                }
		bytes_sent += result;
        }
}

#endif
typedef int ISO_EMPTY_POSIX_CHECK;
