N64_INST=/usr/local/bin
TOOLS=/home/spencer/libdragon/NEW/libdragon/tools

INCS =  -I/usr/local/mips64-elf/include -Iinclude/
WRNGS = -Wall -Wextra -Wpedantic -O4
CFLAGS = -c -std=c++20 -march=vr4300 -mtune=vr4300 $(WRNGS) $(INCS)
DEPS =  main.o libn_timer.o libn_dma_pi.o libn_sprite.o libn_display.o libn_controller.o libn_mempak.o libn_frame.o entry.o

BUILD = $(N64_INST)/mips64-elf-g++ $(CFLAGS) $< -o $@

all: $(DEPS) file.elf file.bin file.z64

entry.o: 			entry.S
	$(BUILD)

main.o: 			main.cpp
	$(BUILD)

libn_timer.o: 		src/libn_timer.cpp
	$(BUILD)

libn_controller.o:  src/libn_controller.cpp
	$(BUILD)

libn_mempak.o:		src/libn_mempak.cpp
	$(BUILD)

libn_frame.o:		src/libn_frame.cpp
	$(BUILD)

libn_display.o: 	src/libn_display.cpp
	$(BUILD)

libn_sprite.o: 		src/libn_sprite.cpp
	$(BUILD)

libn_dma_pi.o:		src/libn_dma_pi.cpp
	$(BUILD)

file.elf: $(DEPS)
	$(N64_INST)/mips64-elf-g++ -o $@ -L/usr/local/mips64-elf/lib/ -L/usr/local/mips64-elf/mips64-elf/lib/ -lm -lc -lgcc  -Tn64.ld *o -Wl,--wrap __do_global_ctors -Wl,--gc-sections -Wl,-Map=main.map

file.bin: file.elf 
	$(N64_INST)/mips64-elf-objcopy $< $@ -O binary

file.z64: file.bin
	$(TOOLS)/n64tool -l 8M -t "LibN Test" -h /home/spencer/libdragon/NEW/libdragon/header -o $@ $< -s 1M sky.sprite 
	$(TOOLS)/chksum64 $@
	
clean:
	rm -f *.bin *.o *.z64 *.elf
