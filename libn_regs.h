
class SI_HANDLER 
{
public:
	void* dram_addr;
	void* pif_addr_r64;
	int r1, r2;
	void* pif_addr_w64;
	int r3;
	int status;
};
static SI_HANDLER *SI_REG = reinterpret_cast<SI_HANDLER*>(SI_ADDRESS);

class VI_HANDLER {
public:
	int status;
	int origin;
	int width;
	int vint;
	int currentvl;
	int vtiming;
	int vsync;
	int hsync;
	int hsyncleap;
	int hvideo;
	int vvideo;
	int vburst;
	int xscale;
	int yscale;
};
static VI_HANDLER *VI_REG = reinterpret_cast<VI_HANDLER*>(VI_ADDRESS);

class PI_HANDLER {
public:
	int dram;
	int cart;
	int rlength;
	int wlength;
	int status;
};
PI_HANDLER *PI_REG = reinterpret_cast<PI_HANDLER*>(PI_ADDRESS);

class MI_HANDLER 
{
public:
    int mode, version, intr, mask;
};

MI_HANDLER *MI_REG = reinterpret_cast<MI_HANDLER*>(0xA4300000);

class DP_HANDLER 
{
public:
    uint32_t *cmd_start;
    uint32_t *cmd_end;
    int cur_address;
    int status;
    int clock_counter;
    int buffer_busy;
    int pipe_busy;
    int tmem_Load_counter;
};

DP_HANDLER *DP_REG = reinterpret_cast<DP_HANDLER*>(0xA4100000);