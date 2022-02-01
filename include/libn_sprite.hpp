#ifndef LIBN_SPRITE_H
#define LIBN_SPRITE_H

#include <libn_types.hpp>
#include <libn_display.hpp>

namespace LibN64 {
class LibSprite {
    Display::LibPos dimensions;
    u32 size;
    u32 *data;

  public:
    LibSprite(const u32 RomOffset);
    void Load(const u32 RomOffset);
    void Draw(Display::LibPos pos);

    u32 Width() const;
    u32 Height() const;
    u32 *Data();
};
} // namespace LibN64

#endif