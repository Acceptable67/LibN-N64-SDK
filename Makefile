ROM_NAME = c_rom
BDIR = build

N64_INST=/usr/local/bin
TOOLS=/home/spencer/libdragon/NEW/libdragon/tools

INCS =  -I/usr/local/mips64-elf/include -Iinclude/
WRNGS = -Wall -Wextra -O2
CFLAGS = -std=c99 -march=vr4300 -mtune=vr4300 -c $(WRNGS) $(INCS) 

DEPS = $(BDIR)/main.o $(BDIR)/timer.o $(BDIR)/stdlib.o $(BDIR)/pi.o $(BDIR)/sprite.o $(BDIR)/vi.o $(BDIR)/si_controller.o $(BDIR)/mempak.o $(BDIR)/vector.o $(BDIR)/sprite.o $(BDIR)/frame.o $(BDIR)/entry.o
BUILD = $(N64_INST)/mips64-elf-gcc $(CFLAGS) $< -o $@
N64TOOL_FILE = -s 1M sky.sprite

all: $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp)) src/entry.S $(BDIR)/$(ROM_NAME).elf $(BDIR)/$(ROM_NAME).bin $(ROM_NAME).z64

#Local
$(BDIR)/main.o: main.c
	$(BUILD)

#Main Build Script
$(BDIR)/%.o: src/%.S
	$(BUILD)

$(BDIR)/%.o: src/%.c
	$(BUILD)

$(BDIR)/$(ROM_NAME).elf: $(DEPS)
	$(N64_INST)/mips64-elf-g++ -o $@ -L/usr/local/mips64-elf/lib/ -L/usr/local/mips64-elf/mips64-elf/lib/ -lm -lc -lgcc -Tinclude/n64.ld $(BDIR)/*.o -Wl,--wrap __do_global_ctors -Wl,--gc-sections -Wl,-Map=main.map

$(BDIR)/$(ROM_NAME).bin: $(BDIR)/$(ROM_NAME).elf 
	$(N64_INST)/mips64-elf-objcopy $< $@ -O binary

$(ROM_NAME).z64: $(BDIR)/$(ROM_NAME).bin
	$(TOOLS)/n64tool -l 2M -t "LibN Test" -h /home/spencer/libdragon/NEW/libdragon/header -o $@ $< $(N64TOOL_FILE) 
	$(TOOLS)/chksum64 $@
	
clean:
	rm -f $(BDIR)/*.bin $(BDIR)/*.o *.z64 $(BDIR)/*.elf
