#ifndef LIBN_SPRITE_H
#define LIBN_SPRITE_H

#include <libn/types.h>
#include <libn/vi.h>
#include <libn/pi.h>

typedef struct {
    u32 width, height;
    u32 size;
    u32 *data;
} LibSprite;

LibSprite* LibSprite_Load(const u32 RomOffset);
void       LibSprite_Draw(LibSprite *spr, const u32 x, const u32 y);
#endif