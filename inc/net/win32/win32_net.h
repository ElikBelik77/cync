#pragma once
#ifdef WIN32
#ifndef WIN32_NET_H_
#define WIN32_NET_H_

#include <winsock2.h>
//#pragma comment(lib, "ws2_32")

#include <windows.h>

#include "structs/queue.h"

/*
	Strcut for NetWorker objects.
*/
typedef struct {
	int port;
	SOCKET sock;
	volatile bool is_running;
	DWORD worker_thread_id;
	HANDLE worker_thread_handle;
	queue_t *in_message_queue;
	queue_t *out_message_queue;
} net_worker_t;


void init_threading(net_worker_t *worker);

#endif
#endif
typedef int silence_iso_win_net;
