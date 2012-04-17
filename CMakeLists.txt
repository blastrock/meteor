cmake_minimum_required(VERSION 2.8)
project(Meteor)

set(METEOR_VERSION_MAJOR 1)
set(METEOR_VERSION_MINOR 4)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

enable_language(CXX)
enable_language(ASM_NASM)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAO REQUIRED ao)
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

set(AMeteor_INCLUDE_DIRS
	${Meteor_SOURCE_DIR}/ameteor/include
)

set(AMeteor_LIBRARY_DIRS
	${Meteor_SOURCE_DIR}/ameteor/source
)

set(AMeteor_LIBRARIES
	ameteor
)

set(MyM_INCLUDE_DIRS
	${Meteor_SOURCE_DIR}/mym/include
	${AMeteor_INCLUDE_DIRS}
	${LIBAO_INCLUDE_DIRS}
)

set(MyM_LIBRARY_DIRS
	${Meteor_SOURCE_DIR}/mym/source
	${AMeteor_LIBRARY_DIRS}
	${LIBAO_LIBRARY_DIRS}
)

set(MyM_LIBRARIES
	mym
	${AMeteor_LIBRARIES}
	GL
	GLU
	GLEW
	pthread
	${LIBAO_LIBRARIES}
	${SFML-SYSTEM_LIBRARY}
	${SFML-WINDOW_LIBRARY}
	${SFML-GRAPHICS_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
)

set(CXXFLAGS CACHE STRING "String to append to CXXFLAGS")
set(LDFLAGS CACHE STRING "String to append to LDFLAGS")

set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_LD_FLAGS "${LDFLAGS}")

if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE "Release")
endif(CMAKE_BUILD_TYPE STREQUAL "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DMETDEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

option(DISABLE_ASM "Force asm optimizations off")

if(NOT DISABLE_ASM AND
		(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR
		CMAKE_SYSTEM_PROCESSOR MATCHES "i.86"))
	message(STATUS "Enabling asm optimizations")
	add_definitions(-DX86_ASM=1)
endif(NOT DISABLE_ASM AND
		(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR
		CMAKE_SYSTEM_PROCESSOR MATCHES "i.86"))

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-parentheses -Wstrict-aliasing=3 -Wmissing-noreturn -pedantic -ansi -pipe")

set(PREFIX_SHARE "${CMAKE_INSTALL_PREFIX}/share/meteor")

configure_file(config.h.in config.h)

add_subdirectory(ameteor)
add_subdirectory(mym)
add_subdirectory(gtkmm)
add_subdirectory(text)
