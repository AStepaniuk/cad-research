add_library(cadgui_editor_operation OBJECT)

file(GLOB OPERATION_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_editor_operation PRIVATE ${OPERATION_SOURCES})

target_include_directories(cadgui_editor_operation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_editor_operation PRIVATE
    corecad
    plan
    cadgui_doc
    cadgui_view
    cadgui_editor_handler
    cadgui_editor_calc_pipeline
)
