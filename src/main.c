#include <stdio.h>
#include "net/net.h"
int main() {
	int blocker;
	NetWorker* worker = init_net_worker(5);
	scanf("%d", &blocker);
	printf("%d data\n", blocker);
}

