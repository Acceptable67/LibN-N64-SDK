#include <libn/controller.h>
#include <libn/types.h>
#include <stdbool.h>
#include <libn/regs.h>
#include <libn/timer.h>
#include <libn/vi.h>
#include <libn/si.h>

s8 SI_READ_MEMPK[64] = {0xFF, 0x03, 0x21, 0x02, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

s8 SI_WRITE_MEMPK[64] = {0xFF, 0x23, 0x01, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

u16 __calculate_address_crc(u16 address) {
    uint16_t xor_table[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x15, 0x1F, 0x0B, 0x16,
        0x19, 0x07, 0x0E, 0x1C, 0x0D, 0x1A, 0x01};
    uint16_t crc           = 0;

    address &= ~0x1F;

    for (int i = 15; i >= 5; i--) {
        if (((address >> i) & 0x1)) { crc ^= xor_table[i]; }
    }

    crc &= 0x1F;

    return address | crc;
}

u8 __calculate_data_crc(u8 *data) {
    uint8_t ret = 0;

    for (int i = 0; i <= 32; i++) {
        for (int j = 7; j >= 0; j--) {
            int tmp = 0;

            if (ret & 0x80) { tmp = 0x85; }

            ret <<= 1;

            if (i < 32) {
                if (data[i] & (0x01 << j)) { ret |= 0x1; }
            }

            ret ^= tmp;
        }
    }

    return ret;
}

u8 local[64];

void __reset_ctrl() {
    Controller_Write();
    Controller_Read();
}

void MemPak_WriteAddress(const u16 address, void *data) {
    u16 address_crc   = __calculate_address_crc(address);
    SI_WRITE_MEMPK[4] = (address_crc >> 8) & 0xFF;
    SI_WRITE_MEMPK[5] = address_crc & 0xFF;

    memcpy(&SI_WRITE_MEMPK[6], (void*)(data), 32);

    SI_Write(&SI_WRITE_MEMPK);
    SI_Read(&local);
    __reset_ctrl();
}

bool MemPak_ReadAddress(const u16 address, void *output) {
    u16 address_crc  = __calculate_address_crc(address);

    SI_READ_MEMPK[4] = (address_crc >> 8) & 0xFF;
    SI_READ_MEMPK[5] = address_crc & 0xFF;

    SI_Write(&SI_READ_MEMPK);
    SI_Read(&local);

    __reset_ctrl();
    if (address_crc == local[5]) {
        memcpy(output, &local[6], 32);
        return true;
    }

    return false;
}

bool MemPak_isInserted() {
    Controller_WriteStatus();

    if (Controller_GetData()[6] == 0x1) {
        __reset_ctrl();
        return true;
    }
    return false;
}