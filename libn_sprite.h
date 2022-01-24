
namespace LibN64 
{
    class LibSprite 
    {
        Display::LibPos dimensions;
        uint32_t size;
        uint32_t *data;

    public:
        LibSprite(uint32_t RomOffset) 
        {
            Load(RomOffset);
        }

        void Load(uint32_t RomOffset) 
        {
            dimensions.x = *reinterpret_cast<short*>(RomOffset);
            dimensions.y = *reinterpret_cast<short*>(RomOffset + (sizeof(char)*2));
            this->size = (dimensions.x * dimensions.y);

            data = new uint32_t[this->size];

            DMA::Read(data, RomOffset + (sizeof(int) * 2), ((this->size) * sizeof(int)) - 0x8);
        }

        void Draw(auto x, auto y) 
        {
            for(uint32_t yy = 0; yy < dimensions.y; yy++) {
                for(uint32_t xx = 0; xx < dimensions.x; xx++) {
                    Display::DrawPixel(x + xx, y + yy, data[yy * dimensions.x + xx]);
                }
            }
        }

        uint32_t Width() 
        {
            return dimensions.x;
        }

        uint32_t Height()
        {
            return dimensions.y;
        }

        uint32_t* Data() 
        {
            return this->data;
        }
    };
};