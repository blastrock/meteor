set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fno-rtti")

add_library(ameteor
	src/audio/dsound.cpp
	src/audio/sound1.cpp
	src/audio/sound2.cpp
	src/audio/sound4.cpp
	src/audio/speaker.cpp
	src/disassembler/argimmediate.cpp
	src/disassembler/argmulregisters.cpp
	src/disassembler/argpsr.cpp
	src/disassembler/argregister.cpp
	src/disassembler/argrelative.cpp
	src/disassembler/argshift.cpp
	src/disassembler/arguimmediate.cpp
	src/disassembler/arguments.cpp
	src/disassembler/instruction.cpp
	src/graphics/bglayer.cpp
	src/graphics/object.cpp
	src/graphics/objects.cpp
	src/graphics/renderer.cpp
	src/graphics/screen.cpp
	src/ameteor.cpp
	src/bios.cpp
	src/cartmem.cpp
	src/clock.cpp
	src/cpu.cpp
	src/debug.cpp
	src/dma.cpp
	src/eeprom.cpp
	src/flash.cpp
	src/interpreter.cpp
	src/interpreter_arm.cpp
	src/interpreter_thumb.cpp
	src/io.cpp
	src/keypad.cpp
	src/lcd.cpp
	src/memory.cpp
	src/sound.cpp
	src/sram.cpp
	src/timer.cpp
)

target_include_directories(ameteor PUBLIC include)
