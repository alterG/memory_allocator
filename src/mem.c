#include "mem.h"
#include "mem_list.h"
#include <sys/syscall.h>



/*check div for size_t*/

static size_t memory_size_round (size_t memory_size) {
	size_t page_size = 4096;
	return memory_size%page_size?memory_size+(page_size-memory_size%page_size):memory_size;
}

void* heap_init(size_t initial_size) {
	void* ptr = mmap(HEAP_START, initial_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	struct mem_t* start_pointer = HEAP_START;
	start_pointer->next = NULL;
	start_pointer->capacity = memory_size_round(initial_size) - sizeof(struct mem_t);
	start_pointer->is_free = 1;
	
	return (char*)start_pointer+sizeof(struct mem_t);	

}

struct mem_t * allocate_block(size_t query) {
	struct mem_t * ptr = mem_t_get_last(HEAP_START);

	/* allocate new block */
	struct mem_t * new_block = mmap((char*)ptr+sizeof(struct mem_t)+(ptr->capacity), query, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	/* if new block is exactly after last block */ 
	if ((char*)new_block == (char*)ptr+sizeof(struct mem_t)+(ptr->capacity) ) {
		printf("After block");
		ptr->capacity += memory_size_round(query);
		return ptr;
	}
	/* if new space is somewhere else */
	else {
		printf("Somewhere block\n");
		new_block->capacity = memory_size_round(query)-sizeof(struct mem_t);
		mem_t_list_insert(mem_t_get_last(HEAP_START), new_block);
		return new_block;
	}
}


/* truncate block capacity (undo first_block_capacity) and divide it into 2 blocks */

void divide_block(struct mem_t * ptr, size_t first_block_capacity) {
	/* min size of 2nd block is its header */
	if ( (ptr->capacity) >= sizeof(struct mem_t)+first_block_capacity ) {
		struct mem_t * sliced;
		sliced = (struct mem_t*)( (char*)ptr+sizeof(struct mem_t)+first_block_capacity );
		sliced->capacity = (ptr->capacity)-first_block_capacity-sizeof(struct mem_t);
		sliced->is_free = 1;
		ptr->capacity = first_block_capacity;
		mem_t_list_insert(ptr, sliced);
	}
}


void* _malloc (size_t query) {

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

/* Merge ptr2 with ptr1 (dont swap!) */

void merge_free_blocks(struct mem_t * ptr1, struct mem_t * ptr2) {
	(ptr1->next) = (ptr2->next);
	ptr1->capacity += sizeof(struct mem_t)+(ptr2->capacity);
}

void _free (void* mem) {
	struct mem_t* ptr;
	struct mem_t* buff_mem = (struct mem_t*)(mem - sizeof(struct mem_t));
	buff_mem->is_free = 1;
	if (buff_mem->next != NULL) {
		ptr = buff_mem->next;
		if (ptr->is_free == 1) merge_free_blocks(buff_mem, ptr);
	}
	if (buff_mem != HEAP_START) {
		ptr = mem_t_get_prev(HEAP_START, buff_mem);
		if (ptr->is_free == 1) merge_free_blocks(ptr, buff_mem);
	}
}

void memalloc_debug_struct_info(FILE* f,
        struct mem_t const* const address){

    size_t i;
    fprintf(f, 
            "start: %p\nsize: %lu\nis_free: %d\n",
            (void*)address, 
            address->capacity, 
            address->is_free);
        for (i = 0; i < DEBUG_FIRST_BYTES && i < address-> capacity; ++i)
            fprintf(f, "%hhX", 
                    ((char*)address)[sizeof(struct mem_t) +i ]);
    putc('\n', f);
}

void memalloc_debug_heap( FILE* f,  struct mem_t const* ptr ) {
    for( ; ptr; ptr = ptr->next) 
        memalloc_debug_struct_info(f, ptr); 
}
