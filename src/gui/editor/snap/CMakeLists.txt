add_library(cadgui_editor_snap OBJECT)

file(GLOB SNAP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_editor_snap PRIVATE ${SNAP_SOURCES})

target_include_directories(cadgui_editor_snap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_editor_snap PRIVATE
    corecad
    plan
    cadgui_doc
    cadgui_view
)

target_link_libraries(cadgui_editor_snap PUBLIC
    cadgui_editor_calc_pipeline
)
