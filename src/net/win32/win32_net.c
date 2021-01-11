#ifdef WIN32
#include "net/net.h"
#include "common.h"
#include "net/win32/win32_net.h"

#include <stdio.h>
void net_read(SOCKET sock, size_t nbytes, void* buffer);
void net_write(SOCKET sock, size_t nbytes, void* buffer);

// Uses select to check sock for clients.
int recv_timeout_tcp(SOCKET socket, long sec, long usec) {
  struct __ms_timeval timeout;
  struct fd_set fds;
  timeout.tv_sec = sec;
  timeout.tv_usec = usec;
  FD_ZERO(&fds);
  FD_SET(socket, &fds);
  return select(0, &fds, 0, 0, &timeout);
}

// Reads a net message from a sock.
NetMessageIn* net_message_read(SOCKET sock) {
  size_t length;
  net_read(sock, sizeof(uint32_t), &length);
  char* payload_buffer = (char*)malloc(length);
  net_read(sock, length, payload_buffer);
  NetMessageIn* net_msg = (NetMessageIn*)malloc(sizeof(NetMessageIn));
  net_msg->payload_size = length;
  net_msg->payload = payload_buffer;
  return net_msg;
}

// Writes a net message to a sock.
void net_message_write(NetMessageOut* net_msg) {
  SOCKET s;
  struct sockaddr_in dest_addr;
  dest_addr.sin_addr.s_addr = inet_addr(net_msg->dest);
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(net_msg->port);
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    fprintf(stderr, "Socket creation failed.\n");
    WSACleanup();
    exit(0);
  }
  if (connect(s, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
    fprintf(stderr, "Socket connection error.\n");
    WSACleanup();
    exit(0);
  }
  char* serialized_msg = net_message_serialize(net_msg);
  size_t serialized_length = sizeof(net_msg->payload_size) + net_msg->payload_size;
  net_write(s, serialized_length, serialized_msg);
  closesocket(s);
}

// Initializes a tcp socket server.
SOCKET init_net_server(NetWorker* context) {
  WSADATA wsa;
  SOCKET s;
  struct sockaddr_in servaddr;
  if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
    fprintf(stderr, "Failed. Error Code : %d\n",WSAGetLastError());
    exit(0);
	}
  else {
    _debug(printf("[!] winsock dll found, status: %s\n", wsa.szSystemStatus));
  }
  if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2) {
    fprintf(stderr, "dll does not suppoerwinsock version.\n");
    WSACleanup();
    exit(0);
  }
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    fprintf(stderr, "Socket create failed.\n");
    WSACleanup();
    exit(0);
  }
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = context->port;
  if (bind(s, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
    fprintf(stderr, "Bind failed.\n");
    closesocket(s);
    WSACleanup();
    exit(0);
  }
  if (listen(s, 5) == SOCKET_ERROR) {
    fprintf(stderr, "Listen failed.\n");
    closesocket(s);
    WSACleanup();
    exit(0);
  }
  return s;
}

// Net worker routine, accept clients and read messages forever.
DWORD WINAPI net_worker_routine(LPVOID lpParam) {
  NetWorker* context = (NetWorker*)lpParam;

  int address_len;
  _debug(printf("[!]: net thread context at: %p\n",context));
  SOCKET s = init_net_server(context);
  while (context->is_running) {
    int retval = recv_timeout_tcp(s, 10, 10);
    if (retval == 0 || retval == -1) {
      continue;
    }
    SOCKET conn = accept(s, NULL, NULL);
    NetMessageIn* cli_msg = net_message_read(conn);
    queue_insert(context->in_message_queue, cli_msg);
    while(!queue_is_empty(context->out_message_queue)) {
      NetMessageOut* out_msg = (NetMessageOut*)queue_pop(context->out_message_queue);
      net_message_write(out_msg);
      net_message_out_free(out_msg);
    }
  }
  closesocket(s);
  return 0;
}
void init_threading(NetWorker* worker) {
  DWORD thread_id;

  HANDLE worker_thread_handle =\
   CreateThread(NULL, 0, net_worker_routine, worker, 0, &thread_id);
  if (worker_thread_handle == NULL) {
    fprintf(stderr, "Error opening thread.");
  }
  worker->worker_thread_handle = worker_thread_handle;
  worker->worker_thread_id = thread_id;
}

void net_worker_free(NetWorker* worker) {
  worker->is_running = 0;
  WaitForSingleObject(worker->worker_thread_handle, INFINITE);
  CloseHandle(worker->worker_thread_handle);
  queue_free(worker->in_message_queue, NULL);
  queue_free(worker->out_message_queue, NULL);
  free(worker);
}

void net_read(SOCKET sock, size_t nbytes, void* buffer) {
  int bytes_read = 0;
  int result;
  while (bytes_read < nbytes) {
    result = recv(sock, buffer + bytes_read, nbytes - bytes_read, 0);
    if (result < 1) {
      fprintf(stderr, "error reading net message.\n");
      exit(0);
    }
    bytes_read += result;
  }
}

void net_write(SOCKET sock, size_t nbytes, void* buffer) {
  int bytes_sent = 0;
  int result;
  while (bytes_sent < nbytes) {
    result = send(sock, buffer + bytes_sent, nbytes - bytes_sent, 0);
    if (result < 1) {
      fprintf(stderr, "error sending net message.\n");
      exit(0);
    }
    bytes_sent += result;
  }
}
#endif
typedef int ISO_EMPTY_WIN_CHECK;
