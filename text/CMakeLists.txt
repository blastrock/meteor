include_directories(
	${MyM_INCLUDE_DIRS}
)

link_directories(
	${MyM_LIBRARY_DIRS}
)

add_executable(meteor_text
	configurator.cpp
	MainText.cpp
	main.cpp
)

target_link_libraries(meteor_text
	${MyM_LIBRARIES}
)
