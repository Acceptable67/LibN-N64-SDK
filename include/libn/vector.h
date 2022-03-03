#ifndef LIBN_VECTOR_H
#define LIBN_VECTOR_H

#include <libn/types.h>

typedef struct {
	void **element;
	s32 pos;
} LibVector;

void  LibVector_Pushback(LibVector *vector, void *item);
void* LibVector_At(LibVector *vector, s32 position);
void  LibVector_EraseAt(LibVector *vector, s32 position);
void  LibVector_Clear(LibVector *vector);
s32   LibVector_Size(LibVector *vector);

#endif