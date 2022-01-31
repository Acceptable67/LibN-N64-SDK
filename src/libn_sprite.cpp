
#include <libn_dma_pi.hpp>
#include <libn_sprite.hpp>
#include <libn_display.hpp>
#include <libn_types.hpp>
#include <cstdint>

namespace LibN64 
{
    LibSprite::LibSprite(const u32 RomOffset) 
    {
        Load(RomOffset);
    }

    void LibSprite::Load(const u32 RomOffset) 
    {
        this->dimensions.x = *reinterpret_cast<short*>(RomOffset);
        this->dimensions.y = *reinterpret_cast<short*>(RomOffset + (sizeof(char)*2));
        this->size = (dimensions.x * dimensions.y);

        data = new u32[this->size];

        DMA::Read(data, RomOffset + (sizeof(int) * 2), ((this->size) * sizeof(int)) - 0x8);
    }

    void LibSprite::Draw(Display::LibPos pos) 
    {
        for(u32 yy = 0; yy < dimensions.y; yy++) {
            for(u32 xx = 0; xx < dimensions.x; xx++) {
               Display::DrawPixel({pos.x + xx, pos.y + yy}, data[yy * dimensions.x + xx]);
            }
        }
    }

    u32 LibSprite::Width() const
    {
        return dimensions.x;
    }

    u32 LibSprite::Height() const
    {
        return dimensions.y;
    }

    u32* LibSprite::Data() 
    {
        return this->data;
    }

};