cmake_minimum_required(VERSION 2.6)
project(Meteor)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

enable_language(CXX)
enable_language(ASM-INTEL)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAO REQUIRED ao)
pkg_check_modules(LIBSIGC++ REQUIRED sigc++-2.0)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
find_library(SFML-GRAPHICS_LIBRARY
	NAMES sfml-graphics
)
find_library(SFML-WINDOW_LIBRARY
	NAMES sfml-window
)
find_library(SFML-SYSTEM_LIBRARY
	NAMES sfml-system
)

if(NOT SFML-GRAPHICS_LIBRARY OR NOT SFML-WINDOW_LIBRARY OR NOT SFML-SYSTEM_LIBRARY)
	message(FATAL_ERROR "You must have SFML to compile this")
endif(NOT SFML-GRAPHICS_LIBRARY OR NOT SFML-WINDOW_LIBRARY OR NOT SFML-SYSTEM_LIBRARY)

set(CMAKE_ASM-INTEL_FLAGS "-f elf64")

set(AMeteor_INCLUDE_DIRS
	${Meteor_SOURCE_DIR}/ameteor/include
	${LIBAO_INCLUDE_DIRS}
	${LIBSIGC++_INCLUDE_DIRS}
	${ZLIB_INCLUDE_DIR}
)

set(AMeteor_LIBRARY_DIRS
	${Meteor_SOURCE_DIR}/ameteor/source
	${LIBAO_LIBRARY_DIRS}
	${LIBSIGC++_LIBRARY_DIRS}
)

set(AMeteor_LIBRARIES
	ameteor
	GL
	GLU
	GLEW
	${LIBAO_LIBRARIES}
	${LIBSIGC++_LIBRARIES}
	${ZLIB_LIBRARY}
	${SFML-GRAPHICS_LIBRARY}
	${SFML-WINDOW_LIBRARY}
	${SFML-SYSTEM_LIBRARY}
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msse3 -march=athlon64 -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msse3 -march=athlon64 -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msse3 -march=athlon64 -fomit-frame-pointer")
add_definitions(-DNDEBUG)
add_definitions(-DX86_ASM=1)
#add_definitions(-DMETDEBUG)
#add_definitions(-DMETDEBUGLOG)
#add_definitions(-DDISASSEMBLE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-parentheses -Wstrict-aliasing=3 -Wmissing-noreturn -pedantic -ansi -pipe")

add_subdirectory(ameteor)
add_subdirectory(gtkmm)
add_subdirectory(text)
add_subdirectory(test)
add_subdirectory(textdisassembler)
