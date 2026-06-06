add_subdirectory(main_window)
add_subdirectory(doc)
add_subdirectory(view)
add_subdirectory(editor)

find_package(OpenGL REQUIRED)

add_executable(
  cadgui
  main.cpp
  ${CADGUI_EDITOR_ALL_OBJECTS}
  $<TARGET_OBJECTS:cadgui_doc>
  $<TARGET_OBJECTS:cadgui_view>
  $<TARGET_OBJECTS:cadgui_main_window>
)

target_include_directories(
  cadgui
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  cadgui
  PRIVATE
  imgui_setup
  corecad
  plan
  cadgui_editor
  cadgui_doc
  cadgui_view
  cadgui_main_window
)
