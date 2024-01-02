N64_INST		= /usr/local/bin

# utilize libdragon tools and header!
TOOLS			= /home/spencer/libdragon/tools
ROM_HEADER  		= /home/spencer/libdragon/header

ROM_NAME = libtest
ROM_SIZE = 2M
BDIR 	 = build
SDIR	 = src

GCC 	= $(N64_INST)/mips64-elf-gcc
CXX 	= $(N64_INST)/mips64-elf-g++
OBJC	= $(N64_INST)/mips64-elf-objcopy
LD	= $(N64_INST)/mips64-elf-ld
N64_TOOL= $(TOOLS)/n64tool
CHKSUM  = $(TOOLS)/chksum64

INCS 	= -I/usr/local/mips64-elf/include 	-Iinclude/
LIBS 	= -L/usr/local/mips64-elf/lib/ 		-L/usr/local/mips64-elf/mips64-elf/lib/ -lc -lm -lgcc
WRNGS 	= -Wall -Wextra -O2
CFLAGS 	= -std=c99	-march=vr4300 -mtune=vr4300 $(WRNGS) $(INCS) 
CXXFLAGS= -std=c++20 	-march=vr4300 -mtune=vr4300 $(WRNGS) $(INCS) 

SRC_FILES = $(BDIR)/main.o $(BDIR)/file.o
ENTRY_SRC = $(SDIR)/entry.S

DEPS 	= 		$(SRC_FILES) 			$(BDIR)/timer.o 		\
			$(BDIR)/stdlib.o 		$(BDIR)/pi.o 			\
			$(BDIR)/sprite.o 		$(BDIR)/vi.o			\
			$(BDIR)/controller.o 		$(BDIR)/mempak.o 		\
			$(BDIR)/si.o 			$(BDIR)/sprite.o 		\
			$(BDIR)/frame.o 		$(BDIR)/vector.o 		\
			$(BDIR)/entry.o 		$(BDIR)/rdp.o 			\
			$(BDIR)/interrupts.o 						\

BUILD 	= $(GCC) -c $(CFLAGS) 	$< -o $@
BCXX	= $(CXX) -c $(CXXFLAGS) $< -o $@

all: $(patsubst $(SDIR)/%.cpp, %.o, $(wildcard $(SDIR)/*.c)) $(ENTRY_SRC) $(BDIR)/$(ROM_NAME).elf $(BDIR)/$(ROM_NAME).bin $(ROM_NAME).z64

# Local #
$(BDIR)/%.o: %.cpp
	@mkdir -p $(BDIR)
	@echo "[BUILD]	$<"
	@$(BCXX)

# Main Build Script #
$(BDIR)/%.o: $(SDIR)/%.S
	@mkdir -p $(BDIR)
	@echo "[BUILD]	$<"
	@$(BUILD)

$(BDIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(BDIR)
	@echo "[BUILD]	$<"
	@$(BUILD)

$(BDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(BDIR)
	@echo "[BUILD]	$<"
	@$(BCXX)

$(BDIR)/$(ROM_NAME).elf: $(DEPS)
	@echo "[LINK]	$@"
	@$(CXX) -o $@ $(LIBS) -Tinclude/n64.ld $(BDIR)/*.o -Wl,--wrap __do_global_ctors -Wl,--gc-sections -Wl,-Map=main.map

$(BDIR)/$(ROM_NAME).bin: $(BDIR)/$(ROM_NAME).elf 
	@echo "[COPY]	$@"
	@$(OBJC) $< $@ -O binary

$(ROM_NAME).z64: $(BDIR)/$(ROM_NAME).bin
	@$(N64_TOOL) -l $(ROM_SIZE) -t "$(ROM_NAME)" -h $(ROM_HEADER) -o $@ $< -s 1M sky.sprite
	@echo "[CHKSUM] $@"
	@$(CHKSUM) $@
	
clean:
	rm -f $(BDIR)/*.bin $(BDIR)/*.o *.z64 $(BDIR)/*.elf *.map 
