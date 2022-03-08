#include <libn/si.h>
#include <libn/controller.h>

u64 SI_READ_MSG[8] = {0xff010401ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                    0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

u64 SI_CTRLR_STATUS[8] ={0xff010300ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                    0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

s8* Controller_GetData() {
	Controller_Read();
	return _SI_Data();
}

void Controller_Write() {

	SI_Write(&SI_READ_MSG);
}

void Controller_Read() {
	SI_Read(_SI_Data());
}

void Controller_WriteStatus() {

	SI_Write(&SI_CTRLR_STATUS);
}
