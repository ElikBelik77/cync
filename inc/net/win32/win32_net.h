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
typedef struct NetWorker_t {
  int port;
	SOCKET sock;
	volatile bool is_running;
	DWORD worker_thread_id;
	HANDLE worker_thread_handle;
	Queue* in_message_queue;
	Queue* out_message_queue;
} NetWorker;


void init_threading(NetWorker* worker);

#endif
#endif
