N64_INST=/usr/local/bin
TOOLS=/home/spencer/libdragon/NEW/libdragon/tools

CFLAGS = -march=vr4300 -mtune=vr4300 -Wall -Wextra -O2  -I/usr/local/mips64-elf/include -I/usr/local/include/ -Iinclude/

all: entry.o main.o file.elf file.bin file.z64

entry.o: entry.S
	$(N64_INST)/mips64-elf-gcc -c -std=gnu99 $(CFLAGS) $< -o $@ 

main.o: main.cpp
	$(N64_INST)/mips64-elf-g++ -c -std=c++20 $(CFLAGS) $< -o $@

file.elf: main.o entry.o
	$(N64_INST)/mips64-elf-g++ -o $@ -L/usr/local/mips64-elf/lib/ -L/usr/local/mips64-elf/mips64-elf/lib/ -L/usr/local/lib/ -lm -lc -lgcc  -Tn64.ld *o -Wl,--wrap __do_global_ctors -Wl,--gc-sections -Wl,-Map=main.map

file.bin: file.elf 
	$(N64_INST)/mips64-elf-objcopy $< $@ -O binary

file.z64: file.bin
	$(TOOLS)/n64tool -l 8M -t "test" -h /home/spencer/libdragon/NEW/libdragon/header -o $@ $< -s 1M sky.sprite 
	$(TOOLS)/chksum64 $@
	
clean:
	rm -f *.bin *.o *.z64 *.elf
