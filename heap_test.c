#include <signal.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Simple program that just allocates on the heap on a SIGINT
 * Used to see how the heap grows with more allocation. Use /proc/<PID>/maps to see where the heap is
 */

static uint64_t bytes_allocated = 0;

#define NALLOC 1 << 12 // 4096 bytes

void int_handler(int sig) {
	malloc(NALLOC);
	bytes_allocated += NALLOC;	
}

int main() {
	while (1) {
		signal(SIGINT, int_handler);
		sleep(1);
		printf("Bytes allocated: %" PRIu64 "\n", bytes_allocated);
	}
}
