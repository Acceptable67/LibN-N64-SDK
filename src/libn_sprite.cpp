
#include <libn_dma_pi.h>
#include <libn_sprite.h>
#include <libn_display.h>
#include <libn_types.h>
#include <cstdint>

namespace LibN64 
{
    LibSprite::LibSprite(const u32 RomOffset) 
    {
        Load(RomOffset);
    }

    void LibSprite::Load(const u32 RomOffset) 
    {
        dimensions.x = *reinterpret_cast<short*>(RomOffset);
        dimensions.y = *reinterpret_cast<short*>(RomOffset + (sizeof(char)*2));
        this->size = (dimensions.x * dimensions.y);

        data = new u32[this->size];

        DMA::Read(data, RomOffset + (sizeof(int) * 2), ((this->size) * sizeof(int)) - 0x8);
    }

    void LibSprite::Draw(u32 x, u32 y) 
    {
        for(u32 yy = 0; yy < dimensions.y; yy++) {
            for(u32 xx = 0; xx < dimensions.x; xx++) {
               Display::DrawPixel({x + xx, y + yy}, data[yy * dimensions.x + xx]);
            }
        }
    }

    u32 LibSprite::Width() const
    {
        return dimensions.x;
    }

    u32 LibSprite::Height()
    {
        return dimensions.y;
    }

    u32* LibSprite::Data() 
    {
        return this->data;
    }

};