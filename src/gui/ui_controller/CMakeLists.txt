add_library(cadgui_ui_controller INTERFACE)

file(GLOB UI_CONTROLLER_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(cadgui_ui_controller INTERFACE ${UI_CONTROLLER_HEADERS})

target_include_directories(cadgui_ui_controller INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
