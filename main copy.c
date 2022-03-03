#include <libn.h>
#include <assert.h>

#define VI_INTR_LOC 0x200

#define SPR_LOC 0xB0101000

#define VWIDTH 320
#define VHEIGHT 240

CreateControllerHandle(CPAD_DATA);

static bool bRunning = false;

void Begin();
void InitDisplay();

u32 x = 0, y = 0;
LibVector vec;
LibSprite *testerSpr;

void InitDisplay() {
	Resolution default_res = {VWIDTH, VHEIGHT};

	Display_Initialize(default_res, BD32BPP, AA_REPLICATE, GAMMA_OFF);
	Display_FillScreen(GREY);
	Display_SetColors(GOLD, NAVY_BLUE | 0xFF);
	Display_SetVI_Intterupt(VI_INTR_LOC);

	Controller_Write();

	bRunning = true;
	testerSpr  = LibSprite_Load(SPR_LOC);
}
 
static const u8 inc_amount = 2;
void Begin() {
	RDP_FillScreen(GREY_SMOOTH);

	s32 pos = 0;
	while(pos < LibVector_Size(&vec)) {
		s32 num = (s32)LibVector_At(&vec, pos);
		LibPrintf("%d", num);
		++pos;
	}

	Display_DrawTri(20,20, 80,20, 60, 60, GREEN);
	//LibSprite_Draw(testerSpr, 40,20);

	LibPrint("Vector and Sprite working.");
	LibPrintf("Height %u Width %u", Display_FrameHeight(), Display_FrameWidth());
	LibPrintf("Current Buffer %p", Display_GetActiveBuffer());
	Display_DrawText(40 + x, 40 + y, "MOVE ME");

	Controller_Read();
	if (CPAD_DATA->A) { Display_DrawText(0, 40, "tester"); }
	if (CPAD_DATA->B) { Display_DrawText(0, 40, "tester B"); }
	if (CPAD_DATA->Z) {}
	if (CPAD_DATA->up) { y -= inc_amount; }
	if (CPAD_DATA->down) { y += inc_amount; }
	if (CPAD_DATA->left) { x -= inc_amount; }
	if (CPAD_DATA->right) { x += inc_amount; }

}

int main() {
	InitDisplay();

	for(s32 index = 0; index < 10; ++index) {
		LibVector_Pushback(&vec, (void*)index);
	}

	while (bRunning) {
		Display_SetVI_IntCallback(&Begin);
		Display_SwapBuffers();
		ResetConsole();
	}
	return EXIT_SUCCESS;
}