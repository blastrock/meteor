find_package(GTK2 2.24 REQUIRED COMPONENTS gtk gtkmm)

include_directories(
	${AMeteor_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
)

link_directories(
	${AMeteor_LIBRARY_DIRS}
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
	${GTK2_LIBRARIES}
)

install(TARGETS meteor DESTINATION bin)

install(
	FILES
		${PROJECT_SOURCE_DIR}/data/meteor.cfg.example
		${PROJECT_SOURCE_DIR}/data/meteor.png
	DESTINATION share/meteor
)
