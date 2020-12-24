add_executable(meteor_text
  audio.cpp
  config.cpp
  events.cpp
  window.cpp
  configurator.cpp
  MainText.cpp
  main.cpp
)

target_link_directories(meteor_text PRIVATE ${LIBAO_LIBRARY_DIRS})
target_include_directories(meteor_text PRIVATE ${LIBAO_INCLUDE_DIRS})

target_include_directories(meteor_text PUBLIC include)

target_link_libraries(meteor_text
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

# remove me
target_link_libraries(meteor_text stdc++fs)
