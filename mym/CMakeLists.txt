set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fno-rtti")

set(mym_SRC
	source/filters/hq4x.cpp

	source/audio.cpp
	source/config.cpp
	source/events.cpp
	source/window.cpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	list(APPEND mym_SRC source/filters/hq4x64.asm)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
	list(APPEND mym_SRC source/filters/hq4x32.asm)
endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")

add_library(mym ${mym_SRC})

target_link_directories(mym PRIVATE ${LIBAO_LIBRARY_DIRS})
target_include_directories(mym PRIVATE ${LIBAO_INCLUDE_DIRS})

target_include_directories(mym PUBLIC include)

target_link_libraries(mym
	ameteor
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
