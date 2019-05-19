#include "alloc.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> //brk, sbrk

extern meta i_meta[10];
extern int t;

void *m_malloc(size_t size){

	if(size == 0)
		return NULL;
	else{
		//printf("malloc size: %d\n", size);
		//printf("t: %d\n", t);
		i_meta[t].free = 0;//true
		i_meta[t].size = size/4*4+4;//aligning the requested size in 4 bytes
		//printf("size: %d\n", i_meta[t].size);
		unsigned int pBrk = sbrk(i_meta[t].size);
		if(pBrk == 0){
			printf("ERROR: sbrk\n");
			return NULL;
		}
		else{
			//printf("brk: %d\n", pBrk);
			return pBrk;
		}
	}

}//Allocation

void m_free(void *ptr){
	if(strcmp(ptr,"")==0){
		return;
	}
	strcpy(ptr,"");
	if(i_meta[t].prev != NULL && i_meta[t-1].free == 1){
		//fusion the two blocks
	}
	if(i_meta[t].next != NULL && i_meta[t+1].free == 1){
		//fusion the two blocks
	}
	if(i_meta[t].next == NULL){
		//release memory
	}
	return;

}//Deallocation

void *m_realloc(void* ptr, size_t size){

}//Reallocation