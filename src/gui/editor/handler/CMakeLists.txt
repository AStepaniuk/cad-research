add_library(cadgui_editor_handler OBJECT)

file(GLOB HANDLER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_editor_handler PRIVATE ${HANDLER_SOURCES})

target_include_directories(cadgui_editor_handler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_editor_handler PRIVATE
    corecad
    plan
    cadgui_doc
    cadgui_view
)

target_link_libraries(cadgui_editor_handler PUBLIC
    cadgui_editor_snap
)
