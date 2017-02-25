#ifndef _MEM_LIST_H
#define _MEM_LIST_H

struct mem_t* mem_t_get_prev(struct  mem_t * start_pointer, struct  mem_t * element);

struct mem_t* mem_t_get_valid_block(struct  mem_t * start_pointer, size_t query); 

void mem_t_list_insert (struct mem_t * start_pointer, struct mem_t * element);

struct mem_t* mem_t_get_last(struct mem_t * start_pointer);

#endif
