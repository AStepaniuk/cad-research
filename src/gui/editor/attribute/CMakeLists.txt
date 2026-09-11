add_library(cadgui_editor_attribute OBJECT)

file(GLOB ATTRIBUTE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_editor_attribute PRIVATE ${ATTRIBUTE_SOURCES})

target_include_directories(cadgui_editor_attribute PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_editor_attribute PRIVATE
    corecad
    plan
    cadgui_localization
)
