#include <libn/interrupt.h>

void Interrupt_VI_SetCallback(void (*callbackFunction)()) {
	if ((MI_REG->intr & MI_REG->mask) & 0x08) {
		callbackFunction();
		VI_REG->currentvl = VI_REG->currentvl;
	}
}

void Interrupt_VI_Toggle(bool toggle, u32 vline) {
	if(toggle) {
		MI_REG->mask = 0x0080;
		VI_REG->vint = vline;
	} else {
		MI_REG->mask = 0x0040;
	}
}
