add_library(cadgui_editor_calc_pipeline OBJECT)

file(GLOB CALC_PIPELINE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_editor_calc_pipeline PRIVATE ${CALC_PIPELINE_SOURCES})

target_include_directories(cadgui_editor_calc_pipeline PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_editor_calc_pipeline PRIVATE
    corecad
    plan
    cadgui_doc
)
