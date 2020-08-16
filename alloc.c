#include <unistd.h>

#include "alloc.h"

#define ALLOC_SIZE 4096

typedef struct free_node free_node_t;

static void *mem_base_ptr = NULL;
static size_t memory_allocated = 0;
static free_node_t *free_list_head = NULL;

static int fetch_more_memory() {
	// Double allocation size each time
	if (sbrk(memory_allocated) != (mem_base_ptr + memory_allocated)) {
		return -1;
	}

	// Init new node for free list
	free_node_t *new_free_node = mem_base_ptr + memory_allocated;
	new_free_node->nfree_bytes = memory_allocated;
	new_free_node->next = NULL;
	
	// Add node to free list
	free_node_t *cur_node = free_list_head;
	while (cur_node->next != NULL) {
		cur_node = cur_node->next;
	}

	cur_node->next = new_free_node;
	new_free_node->prev = cur_node;

	memory_allocated *= 2;

	return 0;
}

static int heap_init() {
	mem_base_ptr = sbrk(0);
	if (sbrk(ALLOC_SIZE) != mem_base_ptr) {
		return -1;
	}

	free_list_head = mem_base_ptr;

	free_list_head->nfree_bytes = ALLOC_SIZE;
	free_list_head->next = NULL;
	free_list_head->prev = NULL;

	return 0;
}

void *alloc(size_t size) {
	// Failed to increment the data segment to it's initial size
	if (free_list_head == NULL && heap_init() != 0) {
		return NULL;
	}

	free_node_t *cur_node = free_list_head;
	//free_node_t *final_node = NULL;

	while (cur_node != NULL) {
		if (cur_node->next == NULL) {
			//final_node = cur_node;
		}
		
		if (cur_node->nfree_bytes >= size) {
			cur_node->nfree_bytes -= size;
			return (void*)cur_node;
		}

		cur_node = cur_node->next;
	}

	fetch_more_memory();
	//new_mem_free_node = final_node->next;


	return NULL;
}

void dealloc(void *ptr) {
	// nop for now
	return;
}
