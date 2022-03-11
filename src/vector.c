#include <libn/vector.h>
#include <libn/vi.h>

void LibVector_Pushback(LibVector *vector, void *item) 
{
	s32 vec_pos = vector->pos;

    if(vec_pos == 0) {
        vector->element = calloc(1, sizeof(vector->element) + sizeof(item));
    } else {
        vector->element = realloc(vector->element, sizeof(vector->element) + sizeof(item));
    }
	vector->element[vec_pos] = item;
	++vector->pos;
}

s32 LibVector_Size(LibVector *vector) {
    return vector->pos;
}

void* LibVector_At(LibVector *vector, s32 position) {
	if(position <= vector->pos)
        return vector->element[position];

    return NULL;
}

void LibVector_EraseAt(LibVector *vector, s32 position) {
    u32 vec_size = sizeof(vector->element[position]);
    if(vec_size >= 1) {
        memset(&vector->element[position], 0, vec_size);
    }
}

void LibVector_Clear(LibVector *vector) {
    for(s32 index = 0; index <= vector->pos; ++index) {
        memset(&vector->element[index], 0, sizeof(vector->element[index]));
    }
    free(vector->element);

    vector->pos = 0;
}