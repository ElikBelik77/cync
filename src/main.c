#include <stdlib.h>
#include <stdio.h>
#include "structs/queue.h"
#if defined(unix) || defined(__unix__) || defined(__unix)
#define _UNIX
#endif

#ifdef _WIN32
int main() {
	printf("win");
}
#endif

#ifdef _UNIX
int main() {
	Queue* queue = queue_create();
	queue_insert(queue, "Hey");
	queue_free(queue, NULL);
}
#endif
