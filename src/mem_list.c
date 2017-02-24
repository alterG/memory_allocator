#include "mem.h"

struct mem_t* mem_t_get_last(struct  mem_t * start_pointer) {
	for( ; start_pointer->next != NULL; start_pointer = start_pointer->next)
	return start_pointer;
}

struct mem_t* mem_t_get_prev(struct  mem_t * start_pointer, struct  mem_t * element) {
	for ( ; start_pointer != NULL && start_pointer->next != element; start_pointer = start_pointer->next)
	return start_pointer;
}

struct mem_t* mem_t_get_valid_block(struct  mem_t * start_pointer, size_t query) {
	for (; start_pointer != NULL &&
		 (start_pointer->is_free = 0 || start_pointer->capacity < query + sizeof(struct mem_t));
	 	start_pointer = start->pointer->next)
	return start_pointer;
}

void mem_t_list_add_front (struct mem_t * start_pointer, struct mem_t * element) {
	element->next = start_pointer;
	start_pointer = element;
} 
