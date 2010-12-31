pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)

include_directories(
	${AMeteor_INCLUDE_DIRS}
	${GLIBMM_INCLUDE_DIRS}
)

link_directories(
	${AMeteor_LIBRARY_DIRS}
	${GLIBMM_LIBRARY_DIRS}
)

add_executable(meteor_text
	configurator.cpp
	MainText.cpp
	main.cpp
)

target_link_libraries(meteor_text
	${AMeteor_LIBRARIES}
	${GLIBMM_LIBRARIES}
)
