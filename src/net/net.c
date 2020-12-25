#include <assert.h>

#include "net.h"
#include "../structs/queue.h"


#if defined(unix) || defined(__unix__) || defined(__unix)
#include <pthread.h>
#include <unistd.h>
void* net_worker_routine(void* arg) {
	NetWorker* context = (NetWorker*)arg;
	while(1) {
		sleep(5);
		printf("Hello from thread\n");
	}
}
pthread_t* init_threading(NetWorker* worker) {
	pthread_t* worker_thread = (pthread_t*)malloc(sizeof(pthread_t));
	assert(worker_thread);
	int result = pthread_create(worker_thread, NULL, net_worker_routine, (void*)worker);
	assert(!result);
	return worker_thread;
}


#endif

#if defined(__WIN32)
// Define windows threading initialization.
#endif

NetWorker* init_net_worker(size_t prot_preamble) {
	NetWorker* worker = (NetWorker*)malloc(sizeof(NetWorker));
	assert(worker);
	worker->in_message_queue  = queue_create();
	worker->out_message_queue = queue_create();
	worker->worker_thread = init_threading(worker);
	return worker;
}

void stop_net_worker(NetWorker* worker) {
	queue_free(worker->in_message_queue, NULL);
	queue_free(worker->out_message_queue, NULL);
	free(worker);
}

