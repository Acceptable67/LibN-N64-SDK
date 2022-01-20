namespace LibN64::DMA 
{
	void Wait() { 
		while(PI_REG->status & 3); 
	} 

	template<class T>
	requires std::is_copy_assignable<T>::value && std::copyable<T>
	void Read(T RAM, int CART, int length) 
	{
		Wait();
		PI_REG->dram = reinterpret_cast<int>(RAM);
		PI_REG->cart = CART & 0x1FFFFFFF;
		PI_REG->wlength = length - 1;
		Wait();
	}

	template<class T, class B>
	requires std::is_copy_assignable<T>::value && std::copyable<T>
		  && std::is_copy_assignable<B>::value && std::copyable<B>
	void Write(T RAM, B CART, int length) 
	{
		Wait();
		PI_REG->dram = reinterpret_cast<int>(RAM);
		PI_REG->cart = reinterpret_cast<int>(CART); //& 0x1FFFFFFF;
		PI_REG->rlength = length - 1;
		Wait();
	}


}


