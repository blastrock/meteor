cmake_minimum_required(VERSION 3.15)
project(Meteor)

set(METEOR_VERSION_MAJOR 1)
set(METEOR_VERSION_MINOR 4)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

enable_language(CXX)
enable_language(ASM_NASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

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
endif()

option(DISABLE_ASM "Force asm optimizations off")
option(METEOR_ASSERTIONS "Enable assertions")
option(METEOR_TRACE "Enable tracing")

if(METEOR_ASSERTIONS)
  add_definitions(-DMETEOR_ASSERTIONS=1)
endif()
if(METEOR_TRACE)
  add_definitions(-DMETEOR_TRACE=1)
endif()

if(NOT DISABLE_ASM AND
		(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR
		CMAKE_SYSTEM_PROCESSOR MATCHES "i.86"))
	message(STATUS "Enabling asm optimizations")
	add_definitions(-DX86_ASM=1)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer")
# RelWithDebInfo is -O2, I need the same as RELEASE to profile
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing=3")
endif()

add_subdirectory(ameteor)
add_subdirectory(mym)
add_subdirectory(text)
