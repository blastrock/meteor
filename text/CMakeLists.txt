pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)

include_directories(
	${MyM_INCLUDE_DIRS}
	${GLIBMM_INCLUDE_DIRS}
)

link_directories(
	${MyM_LIBRARY_DIRS}
	${GLIBMM_LIBRARY_DIRS}
)

add_executable(meteor_text
	configurator.cpp
	MainText.cpp
	main.cpp
)

target_link_libraries(meteor_text
	${MyM_LIBRARIES}
	${GLIBMM_LIBRARIES}
)
