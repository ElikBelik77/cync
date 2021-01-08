#include "net.h"
#include "posix/posix_net.h"
#include "win32/win32_net.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char* net_message_serialize(NetMessageOut* net_msg) {
        char* sr = malloc(sizeof(net_msg->payload_size) + net_msg->payload_size);
        char* ptr = sr;
        memcpy(ptr, &(net_msg->payload_size), sizeof(net_msg->payload_size));
        ptr += sizeof(net_msg->payload_size);
        memcpy(ptr, net_msg->payload, net_msg->payload_size);
        return sr;
}

NetWorker* init_net_worker(int port) {
        NetWorker* worker = (NetWorker*)malloc(sizeof(NetWorker));
        if (worker == NULL) {
		fprintf(stderr, "malloc error.\n");
	}
        worker->is_running = 1;
        worker->in_message_queue  = queue_create();
        worker->out_message_queue = queue_create();
	printf("%p\n",worker->out_message_queue);	
	worker->port = port;
        // Initialize threading, the appropriate function appears i$        // according to the OS.
//      worker->worker_thread = init_threading(worker);
        init_threading(worker);
        return worker;
}



void net_message_in_free(NetMessageIn* net_msg) {
        free(net_msg->payload);
        free(net_msg);
}

void net_message_out_free(NetMessageOut* net_msg) {
        free(net_msg->payload);
        free(net_msg);
}
