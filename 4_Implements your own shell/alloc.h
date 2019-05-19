#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <sys/types.h>

typedef struct meta_struct {

	char* prev, next;
	unsigned int free;
	unsigned int size;

} meta;


void *m_malloc(size_t size);//Allocation

void m_free(void *ptr);//Deallocation

void *m_realloc(void* ptr, size_t size);//Reallocation

#endif
