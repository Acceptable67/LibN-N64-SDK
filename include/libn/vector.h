#ifndef LIBN_VECTOR_H
#define LIBN_VECTOR_H

#include <libn/types.h>

typedef struct {
	void **element;
	s32 pos;
} Vector;

void  Vector_Pushback(Vector *vector, void *item);
void* Vector_At(Vector *vector, s32 position);
void  Vector_EraseAt(Vector *vector, s32 position);
void  Vector_Clear(Vector *vector);
s32   Vector_Size(Vector *vector);

#endif