include_directories(
	${AMeteor_INCLUDE_DIRS}
)

link_directories(
	${AMeteor_LIBRARY_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

add_library(ameteor
	audio/dsound.cpp
	audio/sound1.cpp
	audio/sound2.cpp
	audio/sound4.cpp
	audio/speaker.cpp
	disassembler/argimmediate.cpp
	disassembler/argmulregisters.cpp
	disassembler/argpsr.cpp
	disassembler/argregister.cpp
	disassembler/argrelative.cpp
	disassembler/argshift.cpp
	disassembler/arguimmediate.cpp
	disassembler/arguments.cpp
	disassembler/instruction.cpp
	graphics/filters/hq4x.cpp
	graphics/filters/hq4x64.asm
	graphics/bglayer.cpp
	graphics/object.cpp
	graphics/objects.cpp
	graphics/renderer.cpp
	graphics/screen.cpp
	ameteor.cpp
	bios.cpp
	clock.cpp
	cpu.cpp
	debug.cpp
	dma.cpp
	eeprom.cpp
	events.cpp
	flash.cpp
	interpreter.cpp
	interpreter_arm.cpp
	interpreter_thumb.cpp
	io.cpp
	keypad.cpp
	lcd.cpp
	memory.cpp
	sound.cpp
	sram.cpp
	timer.cpp
)

target_link_libraries(ameteor
	${AMeteor_LIBRARIES}
)
