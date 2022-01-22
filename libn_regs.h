#define FRAMEBUFFER_ADDR 0xA0100000
#define PIF_BASE		 0xBFC00000
#define PIF_RAM  		 0xBFC007C4
#define VI_ADDRESS		 0xA4400000
#define SI_ADDRESS 		 0xA4800000 
#define PI_ADDRESS 		 0xA4600000
#define DP_ADDRESS       0xA4100000
#define MI_ADDRESS       0xA4300000
#define HALT() while(1)

#include <any>

class SI_HANDLER 
{
public:
	std::any* dram_addr;
	std::any* pif_addr_r64;
	uint32_t r1, r2;
	std::any* pif_addr_w64;
	uint32_t r3;
	uint32_t status;
};


class VI_HANDLER {
public:
	uint32_t status;     uint32_t origin;
    uint32_t width;      uint32_t vint;
    uint32_t currentvl;  uint32_t vtiming;
	uint32_t vsync;      uint32_t hsync;
	uint32_t hsyncleap;  uint32_t hvideo;
	uint32_t vvideo;     uint32_t vburst;
	uint32_t xscale;     uint32_t yscale;
};

class PI_HANDLER {
public:
	uint32_t dram;
	uint32_t cart;
	uint32_t rlength;
	uint32_t wlength;
	uint32_t status;
};

class MI_HANDLER 
{
public:
    uint32_t mode, version, intr, mask;
};

class DP_HANDLER 
{
public:
    uint32_t cmd_start;
    uint32_t cmd_end;
    uint32_t cur_address;
    uint32_t status;
    uint32_t clock_counter;
    uint32_t buffer_busy;
    uint32_t pipe_busy;
    uint32_t tmem_Load_counter;
};

static auto SI_REG = reinterpret_cast<SI_HANDLER*>(SI_ADDRESS);
static auto PI_REG = reinterpret_cast<PI_HANDLER*>(PI_ADDRESS);
static auto VI_REG = reinterpret_cast<VI_HANDLER*>(VI_ADDRESS);
static auto MI_REG = reinterpret_cast<MI_HANDLER*>(MI_ADDRESS);
static auto DP_REG = reinterpret_cast<DP_HANDLER*>(DP_ADDRESS);