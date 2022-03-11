N64_INST	= /usr/local/bin
TOOLS		= /home/spencer/libdragon/NEW/libdragon/tools
ROM_HEADER  = /home/spencer/libdragon/NEW/libdragon/header

ROM_NAME = c_rom
ROM_SIZE = 2M
BDIR 	 = build

GCC 	= $(N64_INST)/mips64-elf-gcc
CXX 	= $(N64_INST)/mips64-elf-g++
OBJC	= $(N64_INST)/mips64-elf-objcopy
LD	    = $(N64_INST)/mips64-elf-ld # We're not going to use this but it's handy to have anyway #
N64_TOOL= $(TOOLS)/n64tool
CHKSUM  = $(TOOLS)/chksum64

INCS 	= -I/usr/local/mips64-elf/include -Iinclude/
LIBS 	= -lc -lm -lgcc
WRNGS 	= -Wall -Wextra -O2
CFLAGS 	= -std=c99 -march=vr4300 -mtune=vr4300 $(WRNGS) $(INCS) 
CXXFLAGS= -std=c++20 -march=vr4300 -mtune=vr4300 $(WRNGS) $(INCS) 

SRC_FILES = $(BDIR)/main.o

DEPS 	= 	$(SRC_FILES) $(BDIR)/timer.o \
			$(BDIR)/stdlib.o $(BDIR)/pi.o \
			$(BDIR)/sprite.o $(BDIR)/vi.o \
			$(BDIR)/si.o $(BDIR)/mempak.o \
			$(BDIR)/vector.o $(BDIR)/sprite.o \
			$(BDIR)/entry.o \
			$(BDIR)/rdp.o $(BDIR)/interrupt.o \
			$(BDIR)/controller.o \

BUILD 	= $(GCC) -c $(CFLAGS) $< -o $@
BCXX	= $(CXX) -c $(CXXFLAGS) $< -o $@
N64TOOL_ARGS = -s 1M sky.sprite

all: $(patsubst src/%.cpp, %.o, $(wildcard src/*.c)) src/entry.S $(BDIR)/$(ROM_NAME).elf $(BDIR)/$(ROM_NAME).bin $(ROM_NAME).z64

# Local #
$(BDIR)/%.o: %.c
	@mkdir -p build
	@echo "[BUILD] $<"
	@$(BUILD)

# Main Build Script #
$(BDIR)/%.o: src/%.S
	@mkdir -p build
	@echo "[BUILD] $<"
	@$(BUILD)

$(BDIR)/%.o: src/%.c
	@mkdir -p build
	@echo "[BUILD] $<"
	@$(BUILD)

$(BDIR)/$(ROM_NAME).elf: $(DEPS)
	@echo "[LINK] $@"
	@$(CXX) -o $@ $(LIBS) -Tinclude/n64.ld $(BDIR)/*.o -Wl,--wrap __do_global_ctors -Wl,--gc-sections -Wl,-Map=main.map

$(BDIR)/$(ROM_NAME).bin: $(BDIR)/$(ROM_NAME).elf 
	@echo "[COPY] $@"
	@$(OBJC) $< $@ -O binary

$(ROM_NAME).z64: $(BDIR)/$(ROM_NAME).bin
	@echo "[ROM] $@"
	@$(N64_TOOL) -l $(ROM_SIZE) -t "$(ROM_NAME)" -h $(ROM_HEADER) -o $@ $< $(N64TOOL_ARGS) 
	@$(CHKSUM) $@
	
clean:
	rm -f $(BDIR)/*.bin $(BDIR)/*.o *.z64 $(BDIR)/*.elf *.map 
