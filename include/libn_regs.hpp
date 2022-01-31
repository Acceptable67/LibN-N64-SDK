#ifndef LIBN_REG_H
#define LIBN_REG_H

#define FRAMEBUFFER_ADDR 0xA0100000
#define PIF_BASE		 0xBFC00000
#define PIF_RAM  		 0xBFC007C4

#define VI_ADDRESS		 0xA4400000
#define SI_ADDRESS 		 0xA4800000 
#define PI_ADDRESS 		 0xA4600000
#define DP_ADDRESS       0xA4100000
#define MI_ADDRESS       0xA4300000
#define SP_ADDRESS		 0xA4000000

#define CreateGlobalRegister(GLOB, VARIABLE) \
static volatile auto* VARIABLE = reinterpret_cast<GLOB ## _HANDLER*>(GLOB ## _ADDRESS) \

#include <any>
#include <libn_types.hpp>

struct SI_HANDLER 
{
public:
	std::any* dram_addr;
	std::any* pif_addr_r64;
	u32 r1, r2;
	std::any* pif_addr_w64;
	u32 r3;
	u32 status;
};


struct VI_HANDLER 
{
public:
	u32 status;     u32 origin;
    u32 width;      u32 vint;
    u32 currentvl;  u32 vtiming;
	u32 vsync;      u32 hsync;
	u32 hsyncleap;  u32 hvideo;
	u32 vvideo;     u32 vburst;
	u32 xscale;     u32 yscale;
};

struct PI_HANDLER 
{
public:
	u32 dram;
	u32 cart;
	u32 rlength;
	u32 wlength;
	u32 status;
};

struct MI_HANDLER 
{
public:
    u32 mode, version, intr, mask;
};

struct DP_HANDLER 
{
public:
    u32 cmd_start;
    u32 cmd_end;
    u32 cur_address;
    u32 status;
    u32 clock_counter;
    u32 buffer_busy;
    u32 pipe_busy;
    u32 tmem_Load_counter;
};

struct SP_HANDLER 
{
	u32 SP_DMEM;
	u32 SP_IMEM;
	u32 SP_MEM_ADDR_REG;
	u32 SP_DRAM_ADDR_REG; //for DMA
	u32 SP_RD_LENGTH;
	u32 SP_WRITE_LENGTH;
	u32 SP_STATUS;
	u32 SP_DMA_FULL_REG;
	u32 SP_DMA_BUSY_REG;
};

#endif