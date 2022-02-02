#ifndef LIBN_TYPES_H
#define LIBN_TYPES_H

#include <cstdint>
#include <array>

using u64       = unsigned long long;
using u32       = unsigned int;
using u16       = unsigned short int;
using u8        = unsigned char;

using s64       = long long;
using s32       = int;
using s16       = short int;
using s8        = char;
using byte      = char;

using u64_t     = uint64_t;
using u32_t     = uint32_t;
using u16_t     = uint16_t;
using u8_t      = uint8_t;

using s64_t     = int64_t;
using s32_t     = int32_t;
using s16_t     = int16_t;
using s8_t      = int8_t;

using AnyType = void*;

using PakBuffer = std::array<u8, sizeof(u64) * sizeof(u64)>;

#endif
