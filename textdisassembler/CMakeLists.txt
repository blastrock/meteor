include_directories(
	${AMeteor_INCLUDE_DIRS}
)
link_directories(
	${AMeteor_LIBRARY_DIRS}
)

add_executable(dismeteor
	main.cpp
)

target_link_libraries(dismeteor
	${AMeteor_LIBRARIES}
)
