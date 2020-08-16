#include <unistd.h>

extern void *alloc(size_t size);
extern void dealloc(void *ptr);

struct free_node {
	size_t nfree_bytes;
	struct free_node *next;
	struct free_node *prev;
};
