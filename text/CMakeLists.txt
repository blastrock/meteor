include_directories(
	${AMeteor_INCLUDE_DIRS}
)

link_directories(
	${AMeteor_LIBRARY_DIRS}
)

add_executable(meteor_text
	main.cpp
)

target_link_libraries(meteor_text
	${AMeteor_LIBRARIES}
)
