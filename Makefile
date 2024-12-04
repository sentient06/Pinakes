# Path to RETRO68
RETRO68 := ../../../Retro68-build/toolchain

# Tools
PREFIX := $(RETRO68)/powerpc-apple-macos
CC     := $(RETRO68)/bin/powerpc-apple-macos-gcc
CXX    := $(RETRO68)/bin/powerpc-apple-macos-g++
REZ    := $(RETRO68)/bin/Rez

# Libraries
LIBRARY_LFILES_PATH = ${PWD}/libs/LFiles/lib
INCLUDE_LFILES_PATH = ${PWD}/libs/LFiles/include
CFLAGS   := -I$(INCLUDE_LFILES_PATH) -I${PWD}/src
CXXFLAGS := -I$(INCLUDE_LFILES_PATH) -I${PWD}/src

# Flags
# LDFLAGS   := -L${PWD}/libs/LFiles/lib -lLFiles -lRetroConsole
LDFLAGS   := -lRetroConsole -L$(LIBRARY_LFILES_PATH) -lLFiles 
RINCLUDES := $(PREFIX)/RIncludes
REZFLAGS  := -I$(RINCLUDES)

# Source files
SOURCES := main.cpp src/PIntro.cpp src/PMenu.cpp
OBJECTS := $(SOURCES:.cpp=.o)

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
Pinakes.flt: $(OBJECTS)
	$(CXX) -v $< -o $@ $(LDFLAGS)

# C++ used for linking because RetroConsole needs it

# Implicit rule for object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(BINARIES) Pinakes.flt Pinakes.flt.gdb $(OBJECTS)
