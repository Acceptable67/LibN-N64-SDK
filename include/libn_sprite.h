#ifndef LIBN_SPRITE_H
#define LIBN_SPRITE_H

namespace LibN64 
{
    class LibSprite 
    {
        Display::LibPos dimensions;
        u32 size;
        u32 *data;

    public:
        LibSprite(const u32 RomOffset) 
        {
            Load(RomOffset);
        }

        void Load(const u32 RomOffset) 
        {
            dimensions.x = *reinterpret_cast<short*>(RomOffset);
            dimensions.y = *reinterpret_cast<short*>(RomOffset + (sizeof(char)*2));
            this->size = (dimensions.x * dimensions.y);

            data = new u32[this->size];

            DMA::Read(data, RomOffset + (sizeof(int) * 2), ((this->size) * sizeof(int)) - 0x8);
        }

        void Draw(auto x, auto y) 
        {
            for(u32 yy = 0; yy < dimensions.y; yy++) {
                for(u32 xx = 0; xx < dimensions.x; xx++) {
                    Display::DrawPixel({x + xx, y + yy}, data[yy * dimensions.x + xx]);
                }
            }
        }

        u32 Width() const 
        {
            return dimensions.x;
        }

        u32 Height() const
        {
            return dimensions.y;
        }

        u32* Data()
        {
            return this->data;
        }
    };
};

#endif