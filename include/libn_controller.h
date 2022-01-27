#ifndef LIBN_CONTROLLER_H
#define LIBN_CONTROLLER_H

#include <string.h>
#include <array>
#include <libn_regs.h>

namespace LibN64::Controller 
{

	void _SI_Busy();
	template<class VoidType>
	requires std::is_pointer<VoidType>::value
	void SI_Write(VoidType dram_address);
	template<class VoidType>
	requires std::is_pointer<VoidType>::value
	void SI_Read(VoidType dram_address);

	void SI_WriteController();
	void SI_ReadController();

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

#endif