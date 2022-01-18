
/*controller + SI handling*/
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

static unsigned long long SI_READ_MSG[8] =
{
	0xff010401ffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xfe00000000000000,
	0,
	0,
	1
};
static SI_HANDLER *SI_REG = (SI_HANDLER*)(SI_ADDRESS);

char si_data[64];

namespace LibN64::Controller 
{
	void SI_WriteController() {
		while(SI_REG->status & 3);
		SI_REG->dram_addr = SI_READ_MSG;
		SI_REG->pif_addr_w64 = (void*)0xBFC007C0;
		while(SI_REG->status & 3);
	}

	void SI_ReadController() {
		while(SI_REG->status & 3);
		SI_REG->dram_addr = si_data;
		SI_REG->pif_addr_r64 = (void*)0xBFC007C0;
		while(SI_REG->status & 3);
	}

	enum 
	{
		JOYUP 	= 0x00000072,
		JOYDOWN = 0x0000008E,
		JOYRIGHT= 0x00007200,
		JOYLEFT = 0x00008E00
	};

	struct Cpad
	{
		unsigned A : 1;
		unsigned B : 1;
		unsigned Z : 1;
		unsigned start : 1;
		unsigned up : 1;
		unsigned down : 1;
		unsigned left : 1;
		unsigned right : 1;
		unsigned : 2;
		unsigned L : 1;
		unsigned R : 1;
		unsigned C_up : 1;
		unsigned C_down : 1;
		unsigned C_left : 1;
		unsigned C_right : 1;
		signed x : 8;
		signed y : 8;
	};
}
//for buttons
struct LibN64::Controller::Cpad *cpad_data = reinterpret_cast<LibN64::Controller::Cpad*>(PIF_RAM);

//for joy
int *libn_controller_data = (int*)(PIF_RAM);