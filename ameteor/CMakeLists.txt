set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fno-rtti")

add_library(ameteor
	source/audio/dsound.cpp
	source/audio/sound1.cpp
	source/audio/sound2.cpp
	source/audio/sound4.cpp
	source/audio/speaker.cpp
	source/disassembler/argimmediate.cpp
	source/disassembler/argmulregisters.cpp
	source/disassembler/argpsr.cpp
	source/disassembler/argregister.cpp
	source/disassembler/argrelative.cpp
	source/disassembler/argshift.cpp
	source/disassembler/arguimmediate.cpp
	source/disassembler/arguments.cpp
	source/disassembler/instruction.cpp
	source/graphics/bglayer.cpp
	source/graphics/object.cpp
	source/graphics/objects.cpp
	source/graphics/renderer.cpp
	source/graphics/screen.cpp
	source/ameteor.cpp
	source/bios.cpp
	source/cartmem.cpp
	source/clock.cpp
	source/cpu.cpp
	source/debug.cpp
	source/dma.cpp
	source/eeprom.cpp
	source/flash.cpp
	source/interpreter.cpp
	source/interpreter_arm.cpp
	source/interpreter_thumb.cpp
	source/io.cpp
	source/keypad.cpp
	source/lcd.cpp
	source/memory.cpp
	source/sound.cpp
	source/sram.cpp
	source/timer.cpp
)

target_include_directories(ameteor PUBLIC include)
