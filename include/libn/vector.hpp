#ifndef LIBN_VECTOR_H
#define LIBN_VECTOR_H

#include <libn/types.hpp>

class LibVector {
public:
	void **element;
	s32 pos;

    void  Pushback( void *item);
    void* At( s32 position);
    void  EraseAt( s32 position);
    void  Clear();
    s32   Size();

};



#endif