add_library(cadgui_main_window OBJECT)

file(GLOB MAIN_WINDOW_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_main_window PRIVATE ${MAIN_WINDOW_SOURCES})

target_include_directories(cadgui_main_window PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_main_window PUBLIC
    imgui_setup
)

target_link_libraries(cadgui_main_window PRIVATE
    corecad
    plan
    cadgui_doc
    cadgui_editor
)
