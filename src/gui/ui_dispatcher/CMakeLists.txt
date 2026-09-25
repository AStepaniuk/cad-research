add_library(cadgui_ui_dispatcher INTERFACE)

file(GLOB UI_DISPATCHER_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(cadgui_ui_dispatcher INTERFACE ${UI_DISPATCHER_HEADERS})

target_include_directories(cadgui_ui_dispatcher INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
