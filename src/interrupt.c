#include <libn/interrupt.h>

CreateGlobalRegister(VI, VI_REG);
CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(SI, SI_REG);
CreateGlobalRegister(DP, DP_REG);
CreateGlobalRegister(PI, PI_REG);
CreateGlobalRegister(SP, SP_REG);
CreateGlobalRegister(AI, AI_REG);

typedef struct {
	void (*callback)();
} int_callback;

int_callback callbackList[INT_COUNT];

void Interrupts_ClearCallbacks()
{
	callbackList[INT_VI].callback = 0;
	callbackList[INT_DP].callback = 0;
	callbackList[INT_SI].callback = 0;
	callbackList[INT_PI].callback = 0;
	callbackList[INT_SP].callback = 0;
	callbackList[INT_AI].callback = 0;
}

void Interrupts_SetCallback(IntType type, void (*callbackFunction)()) 
{
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
		callbackList[INT_AI].callback();
		AI_REG->status_reg = 0x00;
	}

	if (reg_stat & 0x01) {
		callbackList[INT_SP].callback();
		SP_REG->SP_STATUS = 0x08;
	}
}


void Interrupts_Toggle(IntType type, bool toggle) 
{
    switch(type) 
    {
		case INT_VI: 
			MI_REG->mask = (toggle) ? 0x80 : 0x40; 
			if(toggle) VI_REG->vint = 0x200;
		 break;
		case INT_PI: MI_REG->mask = (toggle) ? 0x200 : 0x100;  break;
		case INT_SI: MI_REG->mask = (toggle) ? 0x8 	 : 0x4; 	  break;
		case INT_DP: MI_REG->mask = (toggle) ? 0x800 : 0x400;  break;
	    case INT_SP: MI_REG->mask = (toggle) ? 0x2 	 : 0x1; 	break;
		case INT_AI: MI_REG->mask = (toggle) ? 0x20  : 0x10; break;
        default: break;
    };
}