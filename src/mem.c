#include "mem.h"


/*check div for size_t*/

size_t memory_size_round (size_t memory_size) {
	size_t page_size = 4096;
	return memory_size%page_size?memory_size+(page_size-memory_size%page_size):memory_size;
}

void* heap_init(size_t initial_size) {
	void* ptr = mmap(HEAP_START, initial_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	struct mem_t* start_pointer = HEAP_START;
	start_pointer->next = NULL;
	start_pointer->capacity = memory_size_round(initial_size) - sizeof(struct mem_t);
	start-pointer->is_free = 1;
	
	return (char*)start_pointer+sizeof(struct mem_t);	

}

struct mem_t * get_additional_memory(mem_t * ptr, size_t query) {
	ptr = mem_t_get_last(ptr);
	mem_t * new_block = mmap((char*)ptr+sizeof(struct mem_t)+ptr->capacity, query, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	/* if new space is exactly after last block */ 
	if (new_block == (char*)ptr+sizeof(struct mem_t)+ptr->capacity) {
		ptr->capacity += memory_size_round(query);
		return ptr;
	}
	/* if new space is somewhere else */
	else {
		ptr->next = new_block; /*ptr need to be free? */
		new_block->capacity = memory_size_round(query)-sizeof(struct mem_t);
		return new_block;
	}
		
}

void* malloc (size_t query) {

	/*divide block into 2 blocks*/
	struct mem_t * ptr = mem_t_get_valid_block(HEAP_START, query);
	struct mem_t * sliced;
	
	if (ptr)
		sliced = (struct mem_t*)((char*)ptr+sizeof(struct mem_t)+query);
		sliced->capacity = ptr->capacity-query-sizeof(struct mem_t);
		sliced->is_free = 1;
		ptr->capacity = query;
		ptr->is_free = 0;
		mem_t_list_add_front(ptr, sliced);
	else /* try to 1) get space after last block 2)get space somewhere else 3)return NULL */

	return (void*)((char*)ptr+sizeof(struct mem_t));
}
