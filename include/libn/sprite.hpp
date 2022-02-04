#ifndef LIBN_SPRITE_H
#define LIBN_SPRITE_H

#include <libn/types.hpp>

namespace LibN64 {
class LibSprite {
    u32 width, height;
    u32 size;
    u32 *data;

  public:
    LibSprite(const u32 RomOffset);
    void Load(const u32 RomOffset);
    void Draw(const u32, const u32);

    u32 Width() const;
    u32 Height() const;
    u32 *Data();
};
} // namespace LibN64

#endif