include_directories(
	${MyM_INCLUDE_DIRS}
)

link_directories(
	${MyM_LIBRARY_DIRS}
)

set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fno-rtti")

set(mym_SRC
	filters/hq4x.cpp

	audio.cpp
	events.cpp
	window.cpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(mym_SRC ${mym_SRC} filters/hq4x64.asm)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
	set(mym_SRC ${mym_SRC} filters/hq4x32.asm)
endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")

add_library(mym ${mym_SRC})

target_link_libraries(mym
	${MyM_LIBRARIES}
)
