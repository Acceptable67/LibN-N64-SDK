#include <libn.h>
#include <assert.h>

#define VI_INTR_LOC 0x200

#define SPR_LOC 0xB0101000

#define VWIDTH 320
#define VHEIGHT 240

CreateControllerHandle(CPAD_DATA);

static bool bRunning = false;

void Begin();
void Tester();
void InitDisplay();

f32 x = 0, y = 0;
LibVector vec;
LibSprite *testerSpr;
LibTimer *timer;

void InitDisplay() {
	Resolution default_res = {VWIDTH, VHEIGHT};

	Display_Initialize(default_res, BD32BPP, AA_REPLICATE, GAMMA_OFF);
	Display_FillScreen(GREY);
	Display_SetColors(GOLD, NAVY_BLUE | 0xFF);
	Interrupts_Toggle(INT_VI, true);
	Interrupts_SetCallback(INT_VI, &Begin);

	Controller_Write();

	bRunning = true;
	timer = malloc(sizeof(LibTimer) + sizeof(u32));
}
 
static const f32 inc_amount = 0.004;

void Begin() {
	RDP_FillScreen(GREY_SMOOTH);

	s32 pos = 0;
	while(pos < LibVector_Size(&vec)) {
		s32 num = (s32)LibVector_At(&vec, pos);
		LibPrintf("%d", num);
		++pos;
	}

	Display_DrawTri(20,20, 80,20, 60, 60, GREEN);
	Display_DrawRect(80 + (u16)x, 80 + (u16)y, 80, 80, PURPLE, true);

	LibPrint("Vector and Sprite working.");
	LibPrintf("Height %u Width %u", Display_FrameHeight(), Display_FrameWidth());
	LibPrintf("Current Buffer %p", Display_GetActiveBuffer());
	
	Display_DrawText(40 + (s32)x, 40 + (s32)y, "MOVE ME");

	LibPrintf("Cur Timer %u Seconds %u", (u32)timer->ticks / 4, (u32)timer->ticks / 400);
}

int main() {
	InitDisplay();

	for(s32 index = 0; index < 10; ++index) {
		LibVector_Pushback(&vec, (void*)index);
	}

	LibTimer_Setup(timer, TIMER, 0.0f);
	LibTimer_Start(timer);
	while (bRunning) {
		Interrupts_Handle();
		Display_SwapBuffers();
		ResetConsole();

		Controller_Read();
		if (CPAD_DATA->A) { LibPrint("A Pressed"); }
		if (CPAD_DATA->B) { LibPrint("B Pressed"); }
		if (CPAD_DATA->Z) { LibPrint("Z Pressed"); }
		if (CPAD_DATA->start) { LibPrint("Start pressed"); }
		if (CPAD_DATA->up)    { y -= inc_amount; }
		if (CPAD_DATA->down)  { y += inc_amount; }
		if (CPAD_DATA->left)  { x -= inc_amount; }
		if (CPAD_DATA->right) { x += inc_amount; }

		LibTimer_Fetch(timer);
		//LibTimer_Update(&timer, NULL);
	}
	return EXIT_SUCCESS;
}