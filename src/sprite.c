#include <libn.h>

LibSprite* LibSprite_Load(const u32 RomOffset) {
    LibSprite* tmpSpr = malloc(sizeof(LibSprite));

    tmpSpr->width  = *(u16*)(RomOffset);
    tmpSpr->height = *(u16*)(RomOffset + (sizeof(char) * 2));
    tmpSpr->size = (tmpSpr->width * tmpSpr->height);

    tmpSpr->data = malloc(tmpSpr->size + 1);

    DMA_Read(tmpSpr->data, (void*)(RomOffset + (sizeof(u32) * 2)), ((tmpSpr->size) * sizeof(u32)) - 0x8);

        if(!tmpSpr) {
        return NULL;
    }
    
    return tmpSpr;
}

void LibSprite_Draw(LibSprite *spr, const u32 x, const u32 y) {
    	for (u32 yy = 0; yy < spr->height; yy++) {
            for (u32 xx = 0; xx < spr->width; xx++) { 
                Display_DrawPixel(x + xx, y + yy, spr->data[yy * spr->width + xx]); 
        }
	}
}