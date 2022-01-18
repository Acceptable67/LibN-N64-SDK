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
static SI_HANDLER *SI_REG = (SI_HANDLER*)(SI_ADDRESS);

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
static VI_HANDLER *VI_REG = (VI_HANDLER*)(VI_ADDRESS);

class PI_HANDLER {
public:
	int dram;
	int cart;
	int rlength;
	int wlength;
	int status;
};

PI_HANDLER *PI_REG = (PI_HANDLER*)(PI_ADDRESS);

