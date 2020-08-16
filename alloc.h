#include <unistd.h>

extern void *alloc(size_t size);
extern void *resize_alloc(void *ptr, size_t size);
extern void dealloc(void *ptr);

typedef struct mem_node mem_node_t;

struct mem_node {
	void *mem_ptr;
	size_t nbytes;
	unsigned char is_allocated;
	struct mem_node *next;
	struct mem_node *prev;
};
