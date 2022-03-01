#include <libn/vector.h>
#include <libn/vi_display.h>

void Vector_Pushback(Vector *vector, void *item) 
{
	s32 vec_pos = vector->pos;

    if(vec_pos == 0) {
        vector->element = calloc(0, sizeof(vector->element) + sizeof(item));
    } else {
        vector->element = realloc(vector->element, sizeof(vector->element) + sizeof(item));
    }
	vector->element[vec_pos] = item;
	++vector->pos;
}

s32 Vector_Size(Vector *vector) {
    return vector->pos;
}

void* Vector_At(Vector *vector, s32 position) {
	return vector->element[position];
}

void Vector_EraseAt(Vector *vector, s32 position) {
    u32 vec_size = sizeof(vector->element[position]);
    if(vec_size >= 1) {
        memset(&vector->element[position], 0, vec_size);
    }
}

void Vector_Clear(Vector *vector) {
    for(s32 index = 0; index <= vector->pos; ++index) {
        memset(&vector->element[index], 0, sizeof(vector->element[index]));
    }
    free(vector->element);

    vector->pos = 0;
}