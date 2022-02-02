N64_INST=/usr/local/bin
TOOLS=/home/spencer/libdragon/NEW/libdragon/tools

INCS =  -I/usr/local/mips64-elf/include -Iinclude/ -I/
WRNGS = -Wall -Wextra -O3
CFLAGS = -c -std=c++20 -march=vr4300 -mtune=vr4300 $(WRNGS) $(INCS)
DEPS =  main.o libn_timer.o libn_stdlib.o libn_dma_pi.o libn_sprite.o libn_display.o libn_controller.o libn_mempak.o libn_frame.o entry.o

BUILD = $(N64_INST)/mips64-elf-g++ $(CFLAGS) $< -o $@
N64TOOL_FILE = -s 1M sky.sprite

all: $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp)) src/entry.S file.elf file.bin file.z64

#Local
main.o: 			main.cpp
	$(BUILD)

#Main Build Script
%.o: src/%.S
	$(BUILD)

%.o: src/%.cpp
	$(BUILD)

file.elf: $(DEPS)
	$(N64_INST)/mips64-elf-g++ -o $@ -L/usr/local/mips64-elf/lib/ -L/usr/local/mips64-elf/mips64-elf/lib/ -lm -lc -lgcc  -Tinclude/n64.ld *o -Wl,--wrap __do_global_ctors -Wl,--gc-sections -Wl,-Map=main.map

file.bin: file.elf 
	$(N64_INST)/mips64-elf-objcopy $< $@ -O binary

file.z64: file.bin
	$(TOOLS)/n64tool -l 8M -t "LibN Test" -h /home/spencer/libdragon/NEW/libdragon/header -o $@ $< $(N64TOOL_FILE) 
	$(TOOLS)/chksum64 $@
	
clean:
	rm -f *.bin *.o *.z64 *.elf
