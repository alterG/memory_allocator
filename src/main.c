#include "mem.h"

int main(int argc, char** argv){
	heap_init(4096); 
	void *ptr1 = _malloc(1024);
	void *ptr2 = _malloc(1024);
	void *ptr3 = _malloc(1024);
	void *ptr4 = _malloc(1024);
	void *ptr5 = _malloc(1024);
	void *ptr6 = _malloc(1024);
	void *ptr7 = _malloc(1024);
	memalloc_debug_heap(stdout, (void*)(HEAP_START));
	_free(ptr4);
	_free(ptr6);
	_free(ptr5);
	printf(\n"ptr4,5,6 are free.\n");
	memalloc_debug_heap(stdout, (void*)(HEAP_START));
	return 0;
}
