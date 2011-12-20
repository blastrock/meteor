ifeq ($(platform),)
	platform = unix
	ifeq ($(shell uname -a),)
	platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
	platform = osx
else ifneq ($(findstring win,$(shell uname -a)),)
	platform = win
endif
endif

ifeq ($(platform), unix)
	TARGET := libsnes.so
	fpic := -fPIC
	SHARED := -shared -Wl,--version-script=link.T -Wl,--no-undefined
	LIBS := -lz
else ifeq ($(platform), osx)
	TARGET := libsnes.dylib
	fpic := -fPIC
	SHARED := -dynamiclib
	LIBS := -lz
else
	TARGET := snes.dll
	CC = gcc
	CXX = g++
	SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=link.T -Wl,--no-undefined
	CFLAGS += -Izlib
	CXXFLAGS += -Izlib
endif

CFLAGS += -Wall -pedantic -I. -Iinclude -pipe
CXXFLAGS += -Wall -pedantic -I. -Iinclude -pipe

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
	CXXFLAGS += -O0 -g
else
	CFLAGS += -O3
	CXXFLAGS += -O3
endif

SOURCES := libsnes.cpp \
	source/audio/dsound.cpp \
	source/audio/sound1.cpp \
	source/audio/sound2.cpp \
	source/audio/sound4.cpp \
	source/audio/speaker.cpp \
	source/disassembler/argimmediate.cpp \
	source/disassembler/argmulregisters.cpp \
	source/disassembler/argpsr.cpp \
	source/disassembler/argregister.cpp \
	source/disassembler/argrelative.cpp \
	source/disassembler/argshift.cpp \
	source/disassembler/arguimmediate.cpp \
	source/disassembler/arguments.cpp \
	source/disassembler/instruction.cpp \
	source/graphics/bglayer.cpp \
	source/graphics/object.cpp \
	source/graphics/objects.cpp \
	source/graphics/renderer.cpp \
	source/graphics/screen.cpp \
	source/ameteor.cpp \
	source/bios.cpp \
	source/clock.cpp \
	source/cpu.cpp \
	source/debug.cpp \
	source/dma.cpp \
	source/eeprom.cpp \
	source/flash.cpp \
	source/cartmem.cpp \
	source/interpreter.cpp \
	source/interpreter_arm.cpp \
	source/interpreter_thumb.cpp \
	source/io.cpp \
	source/keypad.cpp \
	source/lcd.cpp \
	source/memory.cpp \
	source/sound.cpp \
	source/sram.cpp \
	source/timer.cpp

ZLIB_SOURCES := $(wildcard zlib/*.c)

ifeq ($(platform), win)
	OBJ := $(SOURCES:.cpp=.o) $(ZLIB_SOURCES:.c=.o)
else
	OBJ := $(SOURCES:.cpp=.o)
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(SHARED) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(fpic)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(fpic)

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)

.PHONY: clean
