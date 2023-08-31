#ifndef MPX_MCB_H
#define MPX_MCB_H

#include <stddef.h>
#include <stdlib.h>

typedef struct mcb{
	void* address;
	size_t size;
	int status; // 0 is free, 1 is allocated
	struct mcb* next;
	struct mcb* prev;
}mcb;

extern mcb *mcbList;

void initialize_heap(size_t size);

void *allocate_memory(size_t size);

int free_memory(void* address);
#endif
