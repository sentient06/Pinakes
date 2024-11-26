# Path to RETRO68
RETRO68 := ../../../Retro68-build/toolchain

# Tools
PREFIX := $(RETRO68)/powerpc-apple-macos
CC     := $(RETRO68)/bin/powerpc-apple-macos-gcc
CXX    := $(RETRO68)/bin/powerpc-apple-macos-g++
REZ    := $(RETRO68)/bin/Rez

# Flags
LDFLAGS   := -lRetroConsole
RINCLUDES := $(PREFIX)/RIncludes
REZFLAGS  := -I$(RINCLUDES)

# Targets
BINARIES := Pinakes.bin Pinakes.APPL Pinakes.dsk

all: $(BINARIES)

# Build Pinakes.bin
Pinakes.bin: Pinakes.flt
	$(REZ) $(REZFLAGS) \
		-DFLT_FILE_NAME=\"Pinakes.flt\" "$(RINCLUDES)/Retro68APPL.r" \
		-t APPL -c SWCH \
		-o $@

# Build Pinakes.APPL
Pinakes.APPL: Pinakes.flt
	$(REZ) $(REZFLAGS) \
		-DFLT_FILE_NAME=\"Pinakes.flt\" "$(RINCLUDES)/Retro68APPL.r" \
		--cc $@

# Build Pinakes.dsk
Pinakes.dsk: Pinakes.flt
	$(REZ) $(REZFLAGS) \
		-DFLT_FILE_NAME=\"Pinakes.flt\" "$(RINCLUDES)/Retro68APPL.r" \
		--cc $@

# Link the object file into the flat binary
Pinakes.flt: hello.o
	$(CXX) $< -o $@ $(LDFLAGS)

# C++ used for linking because RetroConsole needs it

# Implicit rule for object file
%.o: %.c
	$(CC) -c $< -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(BINARIES) Pinakes.flt Pinakes.flt.gdb hello.o
