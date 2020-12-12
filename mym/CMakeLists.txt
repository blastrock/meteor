set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fno-rtti")

set(mym_SRC
	source/audio.cpp
	source/config.cpp
	source/events.cpp
	source/window.cpp
)

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
