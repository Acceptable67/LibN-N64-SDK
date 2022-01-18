namespace LibN64::DMA 
{
	void Wait() { 
		while(PI_REG->status & 3); 
	} 

	void Read(int RAM, int CART, int length) 
	{
		Wait();
		PI_REG->dram = RAM & 0x00FFFFFF;
		PI_REG->cart = CART & 0x1FFFFFFF;
		PI_REG->cart = length;
		Wait();
	}
}


