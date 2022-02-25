#ifndef LIBN_MEMPAK_H
#define LIBN_MEMPAK_H

#include <stdbool.h>

bool MemPak_isInserted();
void MemPak_WriteEntry();
void MemPak_ReadEntry();

// unfortunately templates or std::any do not work here so void it is
bool MemPak_ReadAddress(u16, void*);
void MemPak_WriteAddress(u16, void*);

#endif