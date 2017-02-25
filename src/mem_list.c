#include "mem.h"
#include "mem_list.h"

struct mem_t* mem_t_get_last(struct mem_t * start_pointer) {
	while (start_pointer->next != NULL) {
		start_pointer = start_pointer->next;
	}
	return start_pointer;
}


struct mem_t* mem_t_get_prev(struct  mem_t * start_pointer, const struct  mem_t * const  element) {
	while ( (start_pointer != NULL) && (start_pointer->next != element) ) {
		start_pointer = start_pointer->next;
	}
	return start_pointer;
}

struct mem_t* mem_t_get_valid_block(struct  mem_t * start_pointer, const size_t query) {
	do {
		if (( (start_pointer->capacity) >= query + sizeof(struct mem_t) )
			 && (start_pointer->is_free == 1)) break;
		start_pointer = start_pointer->next;
	} while (start_pointer != NULL) ;
	
	return start_pointer;
}

void mem_t_list_insert (struct mem_t * const start_pointer, struct mem_t * const element) {
	element->next = start_pointer->next;
	start_pointer->next = element;
} 
