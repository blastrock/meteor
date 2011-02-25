find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)

include_directories(
	${AMeteor_INCLUDE_DIRS}
	${GTKMM_INCLUDE_DIRS}
)

link_directories(
	${AMeteor_LIBRARY_DIRS}
	${GTKMM_LIBRARY_DIRS}
)

add_executable(meteor
	Configurator.cpp

	AboutDialog.cpp
	DisassemblerTreeView.cpp
	DisassemblerWindow.cpp
	PaletteWindow.cpp
	VramArea.cpp
	VramWindow.cpp
	PadConfigDialog.cpp

	MainWindow.cpp
	main.cpp
)

target_link_libraries(meteor
	${AMeteor_LIBRARIES}
	${GTKMM_LIBRARIES}
)

install(TARGETS meteor DESTINATION bin)

install(
	FILES
		${PROJECT_SOURCE_DIR}/data/meteor.cfg.example
		${PROJECT_SOURCE_DIR}/data/meteor.png
	DESTINATION share/meteor
)
