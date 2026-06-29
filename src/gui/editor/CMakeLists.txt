add_subdirectory(calc_pipeline)
add_subdirectory(handler)
add_subdirectory(operation)
add_subdirectory(snap)

add_library(cadgui_editor OBJECT)

file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_editor PRIVATE ${EDITOR_SOURCES})

set(CADGUI_EDITOR_OBJECTS
    $<TARGET_OBJECTS:cadgui_editor>
    $<TARGET_OBJECTS:cadgui_editor_handler>
    $<TARGET_OBJECTS:cadgui_editor_operation>
    $<TARGET_OBJECTS:cadgui_editor_snap>
    $<TARGET_OBJECTS:cadgui_editor_calc_pipeline>
)

set(CADGUI_EDITOR_ALL_OBJECTS ${CADGUI_EDITOR_OBJECTS} PARENT_SCOPE)


target_include_directories(cadgui_editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cadgui_editor PRIVATE
    corecad
    imgui_setup
)
target_link_libraries(cadgui_editor PUBLIC
    cadgui_doc
    cadgui_editor_handler
    cadgui_editor_operation
    cadgui_editor_snap
    cadgui_editor_calc_pipeline
    cadgui_view
)
