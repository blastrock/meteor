include_directories(
	${AMeteor_INCLUDE_DIRS}
)

link_directories(
	${AMeteor_LIBRARY_DIRS}
)

add_executable(meteor_test
	main.cpp
)

target_link_libraries(meteor_test
	${AMeteor_LIBRARIES}
)
