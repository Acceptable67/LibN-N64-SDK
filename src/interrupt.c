#include <libn/interrupt.h>

CreateGlobalRegister(VI, VI_REG);
CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(SI, SI_REG);
CreateGlobalRegister(DP, DP_REG);
CreateGlobalRegister(PI, PI_REG);
CreateGlobalRegister(SP, SP_REG);

typedef struct {
	void (*callback)();
} int_callback;

int_callback callbackList[7];

void Interrupts_Clear()
{
	callbackList[INT_VI].callback = 0;
	callbackList[INT_DP].callback = 0;
	callbackList[INT_SI].callback = 0;
	callbackList[INT_PI].callback = 0;
}

void Interrupts_Disable()
{
	__asm__("\tmfc0 $8,$12\n\tla $9,~1\n\tand $8,$9\n\tmtc0 $8,$12\n\tnop":::"$8","$9");
}

void Interrupts_SetCallback(IntType type, void (*callbackFunction)()) {
		callbackList[type].callback = callbackFunction;
}

void Interrupts_Handle() 
{	
	u32 reg_stat = (MI_REG->intr & MI_REG->mask);
	if (reg_stat & 0x08) { //VI
		callbackList[INT_VI].callback();
		VI_REG->currentvl = VI_REG->currentvl;
	}

	if (reg_stat & 0x02) { //SI
		callbackList[INT_SI].callback();
		SI_REG->status = 0x0;
	}

	if (reg_stat & 0x10) { //PI
		callbackList[INT_PI].callback();
		PI_REG->status = 0x2;
	}

	if (reg_stat & 0x20) { //DP
		callbackList[INT_DP].callback();
		MI_REG->mode = 0x800;
	}

	if (reg_stat & 0x04) {
		//callbackList[INT_AI].callback();
	}

	if (reg_stat & 0x01) {
		callbackList[INT_SP].callback();
		SP_REG->SP_STATUS = 0x08;
	}
}


void Interrupts_Toggle(IntType type, bool toggle) {
    switch(type) 
    {
		case INT_VI: 
		{
			if(toggle) {
				MI_REG->mask = 0x0080;
				VI_REG->vint = 0x200;
			} else {
				MI_REG->mask = 0x0040;
			}	
		} break;
		case INT_PI: 
		{
			if (toggle) {
				MI_REG->mask=0x200;
			} else	{
				MI_REG->mask=0x100;
			}	
		} break;
		case INT_SI: 
		{
			if(toggle) {
				MI_REG->mask = 0x8;
			} else {
				MI_REG->mask = 0x4;
			}
		} break;
		case INT_DP: 
		{
			if(toggle) {
				MI_REG->mask = 0x800;
			} else {
				MI_REG->mask = 0x400;
			}
		} break;
	    case INT_SP: 
		{
			if(toggle) { 
				MI_REG->mask = 0x2;
			} else {
				MI_REG->mask = 0x1;
			}
		} break;
        default: break;
    };
}