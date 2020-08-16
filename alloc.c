#include <string.h>
#include <unistd.h>

#include "alloc.h"

#define ALLOC_SIZE (4096ULL)

static void *mem_base_ptr = NULL;
static size_t memory_allocated = 0;
static mem_node_t *mem_list = NULL;

static mem_node_t *init_mem_node(void *addr, size_t size, unsigned char is_allocated) {
	mem_node_t *node = addr;

	node->mem_ptr = mem_base_ptr + sizeof(mem_node_t);
	node->nbytes = size - sizeof(mem_node_t);
	node->is_allocated = is_allocated;
	node->next = NULL;
	node->prev = NULL;
	
	return node;
}

static size_t bytes_for_4k_alignment(size_t nbytes) {
	size_t bytes_to_alloc = 0;
	while (bytes_to_alloc < nbytes) {
		bytes_to_alloc += ALLOC_SIZE;
	}
	return bytes_to_alloc;
}

static int fetch_more_memory(size_t size) {
	size_t bytes_to_fetch = bytes_for_4k_alignment(size) + sizeof(mem_node_t);
	if (sbrk(bytes_to_fetch) != (mem_base_ptr + memory_allocated)) {
		return -1;
	}

	// Init new node for free list
	mem_node_t *new_free_node = init_mem_node(mem_base_ptr + memory_allocated, bytes_to_fetch, 0);
	
	// Add node to free list
	mem_node_t *cur_node = mem_list;
	while (cur_node->next != NULL) {
		cur_node = cur_node->next;
	}

	cur_node->next = new_free_node;
	new_free_node->prev = cur_node;

	memory_allocated += bytes_to_fetch;

	return 0;
}

static int heap_init() {
	mem_base_ptr = sbrk(0);
	if (sbrk(ALLOC_SIZE) != mem_base_ptr) {
		return -1;
	}

	mem_list = init_mem_node(mem_base_ptr, ALLOC_SIZE, 0);

	memory_allocated += ALLOC_SIZE;
	
	return 0;
}

void *alloc(size_t size) {
	// Failed to increment the data segment to it's initial size
	if (mem_list == NULL && heap_init() != 0) {
		return NULL;
	}

	mem_node_t *cur_node = mem_list;
	mem_node_t *final_node = NULL;

	while (cur_node != NULL) {
		if (cur_node->next == NULL) {
			final_node = cur_node;
		}
		
		if (cur_node->nbytes >= size && cur_node->is_allocated == 0) {
			size_t remaining_bytes = cur_node->nbytes - size;
			if (remaining_bytes > sizeof(mem_node_t)) {
				cur_node->nbytes = size;
				cur_node->is_allocated = 1;

				mem_node_t *new_node = init_mem_node(cur_node->mem_ptr + size, remaining_bytes, 0);

				mem_node_t *tmp_node = cur_node->next;

				new_node->prev = cur_node;
				new_node->next = tmp_node;

				cur_node->next = new_node;

				if (tmp_node != NULL) {
					tmp_node->prev = new_node;
				}

				return (void*)cur_node->mem_ptr;
			}
		}

		cur_node = cur_node->next;
	}

	if (fetch_more_memory(size) == -1) {
		return NULL;
	}

	mem_node_t *new_mem_node = final_node->next;
	
	size_t remaining_bytes = new_mem_node->nbytes - size;
	new_mem_node->nbytes = size;
	new_mem_node->is_allocated = 1;

	if (remaining_bytes > 0) {
		mem_node_t *remaining_mem_node = init_mem_node(new_mem_node->mem_ptr + size, remaining_bytes, 0);

		new_mem_node->next = remaining_mem_node;
		remaining_mem_node->prev = new_mem_node;
	}

	return (void*)new_mem_node->mem_ptr;	
}

void dealloc(void *ptr) {
	mem_node_t *node = mem_list;
	while (node != NULL) {
		if (ptr == node->mem_ptr) {
			node->is_allocated = 0;
			break;
		}
		node = node->next;
	}
}


//static void defrag() {
//
//}

void *resize_alloc(void *ptr, size_t size) {
	if (size == 0) {
		return NULL;
	}

	int found = 0;
	mem_node_t *node = mem_list;
	while (node != NULL) {
		if (node->mem_ptr == ptr) {
			found = 1;
			break;
		}
		node = node->next;
	}

	if (found == 0) {
		return NULL;
	}

	void *new_ptr = alloc(size);
	if (new_ptr != NULL) {
		return NULL;
	}
	
	// Want to copy the minimum to prevent overflow in the new allocation and to prevent copying stuff that isn't in the old allocation
	size_t copy_amount = size < node->nbytes ? size : node->nbytes;

	memcpy(new_ptr, ptr, copy_amount);

	dealloc(ptr);
	return new_ptr;
}
