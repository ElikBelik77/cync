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

#include "posix_net.h"
#include "../net.h"
#include "../../structs/queue.h"

void net_read(int sockfd, size_t nbytes, void* buffer);
void net_write(int sockfd, size_t nbytes, void* buffer);

// Writes a given network message to a socket fd.
void net_message_write(NetMessageOut* net_msg) {
	struct hostent* he;
	struct sockaddr_in dest_addr;
	int sockfd;
	memset(&dest_addr, sizeof(struct sockaddr_in), 0);
	if ((he = gethostbyname(net_msg->dest)) == NULL) {
		fprintf(stderr, "Invalid destination.\n");
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		fprintf(stderr, "socket open error.\n");
	}
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(net_msg->port);
	dest_addr.sin_addr.s_addr = inet_addr(net_msg->dest);
	if (connect(sockfd, (struct sockaddr *)&dest_addr, \
					sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "socket connection error.\n");
	}
	char* serialized_msg = net_message_serialize(net_msg);
	size_t serialized_length = sizeof(net_msg->payload_size) + net_msg->payload_size;
	net_write(sockfd, serialized_msg, serialized_length);
	close(sockfd);
}

// Read a network message from a given socket fd.
NetMessageIn* net_message_read(int sockfd) {
	// Reads length, then read length bytes.
	size_t length;
	net_read(sockfd, sizeof(uint32_t), &length);
	char* payload_buffer = (char*)malloc(length);
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
		NetMessageIn* cli_msg = net_message_read(connfd);
		queue_insert(context->in_message_queue, cli_msg);
		while (!queue_is_empty(context->out_message_queue)) {
			NetMessageOut* out_msg = (NetMessageOut*)queue_pop(context->out_message_queue);
			net_message_write(out_msg);
			net_message_out_free(out_msg);
		}
	}
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
