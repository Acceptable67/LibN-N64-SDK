#ifndef LIBN_VECTOR_H
#define LIBN_VECTOR_H

#include <libn/types.hpp>

class LibVector {
public:
	void **element;
	s32 pos;

    void  push_back(void *item);
    void* at(s32 position);
    void  erase_at(s32 position);
    void  clear();
    s32   size();

};

#endif
