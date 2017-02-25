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

struct mem_t * allocate_block(size_t query) {
	mem_t * ptr = mem_t_get_last(HEAP_START);

	/* allocate new block */
	mem_t * new_block = mmap((char*)ptr+sizeof(struct mem_t)+(ptr->capacity), query, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	
	if (new_block == NULL) return NULL;

	/* if new block is exactly after last block */ 
	if (new_block == (char*)ptr+sizeof(struct mem_t)+(ptr->capacity) ) {
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


/* truncate block capacity (undo first_block_capacity) and divide it into 2 blocks */

void divide_block(struct mem_t * ptr, size_t first_block_capacity) {
	/* min size of 2nd block is its header */
	if ( (ptr->capacity) >= sizeof(mem_t)+first_block_capacity ) {
		struct mem_t * sliced;
		sliced = (struct mem_t*)( (char*)ptr+sizeof(struct mem_t)+first_block_capacity );
		sliced->capacity = (ptr->capacity)-first_block_capacity-sizeof(struct mem_t);
		sliced->is_free = 1;
		ptr->capacity = first_block_capacity;
		mem_t_list_insert(ptr, sliced);
	}
}


void* malloc (size_t query) {

	/* try to find nessesary block in heap */
	struct mem_t * ptr = mem_t_get_valid_block(HEAP_START, query);
	
	/* if heap hasnt nessesary block try to allocate new block */
	if (ptr == NULL) {
		ptr = allocate_block(query);
		if (ptr == NULL) return NULL;
	}
	ptr->is_free = 0;
	/* optimaze block capacity */
	divide_block(ptr, query);
	return (void*)((char*)ptr+sizeof(struct mem_t));
}
