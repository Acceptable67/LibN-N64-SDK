#include <libn.h>
#include <libn/vector.h>

CreateControllerHandle(CPAD_DATA);

static const Resolution default_res = {320, 240};
static bool bRunning		    = false;
static const u8 inc_amount	    = 2;

void InitDisplay() {
	Display_Initialize(default_res, BD32BPP, AA_REPLICATE, GAMMA_OFF);
	Display_FillScreen(GREY);
	Display_SetColors(GOLD, NAVY_BLUE | 0xFF);

	bRunning = true;

	Display_SetVI_Intterupt(0x200);
	Controller_Write();
}

u32 x = 0, y = 0;
void CheckController() {
	Controller_Read();
	if (CPAD_DATA->A) { Display_DrawText(0, 40, "tester  "); }
	if (CPAD_DATA->B) { Display_DrawText(0, 40, "tester B"); }
	if (CPAD_DATA->Z) {}
	if (CPAD_DATA->up) { y -= inc_amount; }
	if (CPAD_DATA->down) { y += inc_amount; }
	if (CPAD_DATA->left) { x -= inc_amount; }
	if (CPAD_DATA->right) { x += inc_amount; }
}

Vector vec;

void main_func() {
	RDP_FillScreen(GREY_SMOOTH);

	const s8 *name = (const s8 *)Vector_At(&vec, 1);
	printf("%s", name);

	Display_DrawText(40 + x, 40 + y, "MOVE ME");

	CheckController();
}

void VecAddItem(void *item) {
	Vector_Pushback(&vec, item);
}

int main() {
	InitDisplay();

	VecAddItem((void *)0x12345678);
	VecAddItem("tester");
	VecAddItem("tester2");

	while (bRunning) {
		Display_SetVI_IntCallback(&main_func);
		ResetConsole();
	}
	return EXIT_SUCCESS;
}