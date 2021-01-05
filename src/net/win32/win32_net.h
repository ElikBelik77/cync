#if defined(WIN32)
#ifndef WIN32_NET_H_
#define WIN32_NET_H_

#include <windows.h>

typedef struct NetWorker_t {
	int sock_fd, port;
	volatile bool is_running;
	DWORD worker_thread_id;
	HANDLE worker_thread_handle;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker;

void init_threading(NetWorker* worker);

#endif
#endif
