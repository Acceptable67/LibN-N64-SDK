#include <libn.h>
#define VI_INTR_LOC 0x200

#define VWIDTH 320
#define VHEIGHT 240

CreateControllerHandle(CPAD_DATA);

static bool bRunning = true;
static const u8 inc_amount = 4;
s32 x, y;

void Begin();
void InitDisplay();
void CheckController();

void InitDisplay() {
	Resolution default_res = {VWIDTH, VHEIGHT};

	Display_Initialize(default_res, BD32BPP, AA_RESAMP_FETCH, GAMMA_OFF);
	Display_FillScreen(GREY);
	Display_SetColors(GOLD, NAVY_BLUE | 0xFF);

	Interrupts_Toggle(INT_VI, true);
	
	Interrupts_SetCallback(INT_VI, &Begin); //Function called every vblank

	Controller_Enable();
}


void CheckController()
{
	Controller_Read();
	if (CPAD_DATA->A) { LibPrint("tester"); }
	if (CPAD_DATA->B) { LibPrint("tester B"); }
	if (CPAD_DATA->Z) {}
	if (CPAD_DATA->up) { y -= inc_amount; }
	if (CPAD_DATA->down) { y += inc_amount; }
	if (CPAD_DATA->left) { x -= inc_amount; }
	if (CPAD_DATA->right) { x += inc_amount; }
}
 
void Begin() {
		
	RDP_FillScreen(GREY_SMOOTH);
	LibPrint("tester1\ntester2");
	//RDP_Debug();
	Display_DrawRect(20 + x, 20 + y, 40, 40, RED, false);
	//Display_DrawCircle(40, 40, 6, ORANGE, false, 0.1);
	CheckController();
	ResetConsole();

}

void tester() {
	LibPrint("testing");
}

int main() {
	InitDisplay();
	while (bRunning) {
		Interrupts_Handle();
		Display_SwapBuffers();
	} 
	return EXIT_SUCCESS;
}