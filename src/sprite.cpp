
#include <cstdint>
#include <libn/pi.hpp>
#include <libn/sprite.hpp>
#include <libn/types.hpp>
#include <libn/vi.hpp>

namespace LibN64 {
LibSprite::LibSprite(const u32 RomOffset) {
	Load(RomOffset);
}

void LibSprite::Load(const u32 RomOffset) {
	this->width  = *reinterpret_cast<short *>(RomOffset);
	this->height = *reinterpret_cast<short *>(RomOffset + (sizeof(char) * 2));
	this->size   = (width * height);

	data	     = new u32[this->size];

	DMA::Read(data, RomOffset + (sizeof(int) * 2), ((this->size) * sizeof(int)) - 0x8);
}

void LibSprite::Draw(u32 x, u32 y) {
	for (u32 yy = 0; yy < height; yy++) {
		for (u32 xx = 0; xx < width; xx++) { Display::DrawPixel({x + xx, y + yy}, data[yy * width + xx]); }
	}
}

u32 LibSprite::Width() const {
	return width;
}

u32 LibSprite::Height() const {
	return height;
}

u32 *LibSprite::Data() {
	return this->data;
}

}; // namespace LibN64