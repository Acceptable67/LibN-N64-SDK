#ifndef LIBN_RDP_H
#define LIBN_RDP_H

#include <libn.hpp>

namespace RDP {
    enum Command {
        DL_ENABLE_PRIM	  = 0xEFB000FF,
        DL_ENABLE_PRIM_2  = 0x00004000,
        DL_ENABLE_BLEND_2 = 0x80000000,
        DL_ENABLE_BLEND	  = 0xEF0000FF,
        DL_SET_PRIM_COL	  = 0xF7000000,
        DL_SET_BLEND_COL  = 0xF9000000,
        DL_DRAW_RECT	  = 0xF6000000,
        DL_ATTACH_FB	  = 0x3F000000,
        DL_SET_CLIP_AREA  = 0xED000000,
        DL_SYNC_PIPE	  = 0xE7000000,
        DL_NULL_CMD	  = 0x00000000
    };

    void DebugAddr();
    void SendDisplayList();
    void AddCommand(u32);
    void Send();
    void Sync();
    void Attach();
    void EnableBlend();
    void EnablePrimitive();
    void SetDefaultClipping();
    void SetClipping(u32, u32, u32, u32);
    void SetBlendColor(const auto);
    void SetPrimitiveColor(const auto);
    void DrawRectangle(u32 point1, u32 point2, u32 width, u32 heiht);
    void DrawRectangleSetup(u32 tx, u32 ty, u32 bx, u32 by, u32 color);
    void Close();
    void FillScreen(u32 color);
    void Init();
    // void LoadTexture(LibSprite, u32);
    } // namespace RDP

#endif