add_library(cadgui_editor_calc_pipeline INTERFACE)

#file(GLOB HANDLER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

#target_sources(cadgui_editor_handler PRIVATE ${HANDLER_SOURCES})

target_include_directories(cadgui_editor_calc_pipeline INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_editor_calc_pipeline INTERFACE
    corecad
    plan
    cadgui_doc
)
