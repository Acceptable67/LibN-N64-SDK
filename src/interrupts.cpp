#include <libn/interrupts.hpp>

CreateGlobalRegister(VI, VI_REG);
CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(SI, SI_REG);
CreateGlobalRegister(DP, DP_REG);
CreateGlobalRegister(PI, PI_REG);
CreateGlobalRegister(SP, SP_REG);
CreateGlobalRegister(AI, AI_REG);


/*void ClearCallbacks()
{
	callbackList[Interrupts::Type::VI].callback = 0;
	callbackList[Interrupts::Type::DP].callback = 0;
	callbackList[Interrupts::Type::SI].callback = 0;
	callbackList[Interrupts::Type::PI].callback = 0;
	callbackList[Interrupts::Type::SP].callback = 0;
	callbackList[Interrupts::Type::AI].callback = 0;
}*/

namespace Interrupts 
{
	std::function<void()> callbackList[INT_COUNT];

	void SetCallback(Interrupts::Type type, std::function<void()> callbackFunction) 
	{
		callbackList[type] = callbackFunction;
	}

		void SetCallback(Interrupts::Type type, void(*callbackFunction)()) 
	{
		std::function<void()> tmp = callbackFunction;
		callbackList[type] = tmp;
	}


	void Handle() 
	{	
		u32 reg_stat = (MI_REG->intr & MI_REG->mask);
		if (reg_stat & 0x08) { //VI
			callbackList[Interrupts::Type::VI]();
			VI_REG->currentvl = VI_REG->currentvl;
		}

		if (reg_stat & 0x02) { //SI
			callbackList[Interrupts::Type::SI]();
			SI_REG->status = 0x0;
		}

		if (reg_stat & 0x10) { //PI
			callbackList[Interrupts::Type::PI]();
			PI_REG->status = 0x2;
		}

		if (reg_stat & 0x20) { //DP
			callbackList[Interrupts::Type::DP]();
			MI_REG->mode = 0x800;
		}

		if (reg_stat & 0x04) {
			callbackList[Interrupts::Type::AI]();
			AI_REG->status_reg = 0x00;
		}

		if (reg_stat & 0x01) {
			callbackList[Interrupts::Type::SP]();
			SP_REG->status_reg = 0x08;
		}
	}


	void Toggle(Interrupts::Type type, bool toggle) 
	{
		switch(type) 
		{
			case Interrupts::Type::VI:
			{
				MI_REG->mask = (toggle) ? 0x80 : 0x40; 
				if(toggle) VI_REG->vint = 0x200;
			}
			break;
			case Interrupts::Type::PI: MI_REG->mask = (toggle) ? 0x200 : 0x100;  break;
			case Interrupts::Type::SI: MI_REG->mask = (toggle) ? 0x8 	 : 0x4; 	  break;
			case Interrupts::Type::DP: MI_REG->mask = (toggle) ? 0x800 : 0x400;  break;
			case Interrupts::Type::SP: MI_REG->mask = (toggle) ? 0x2 	 : 0x1; 	break;
			case Interrupts::Type::AI: MI_REG->mask = (toggle) ? 0x20  : 0x10; break;
			default: break;
		};
	}
}