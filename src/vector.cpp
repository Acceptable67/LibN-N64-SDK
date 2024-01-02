#include <libn/vector.hpp>
#include <libn/vi.hpp>
#include <stdlib.h>
#include <cstring>

void LibVector::push_back(void *item) 
{
	s32 vec_pos = this->pos;

    this->element = (void**)malloc(sizeof(this->element) + sizeof(item));

	this->element[vec_pos] = item;
	++this->pos;
}

s32 LibVector::size() {
    return this->pos;
}

void* LibVector::at(s32 position) {
	if(position <= this->pos)
        return this->element[position];

    return NULL;
}

void LibVector::erase_at(s32 position) {
    u32 vec_size = sizeof(this->element[position]);
    if(vec_size >= 1) {
        memset(&this->element[position], 0, vec_size);
    }
}

void LibVector::clear() {
    for(s32 index = 0; index <= this->pos; ++index) {
        memset(&this->element[index], 0, sizeof(this->element[index]));
    }
    free(this->element);

    this->pos = 0;
}
