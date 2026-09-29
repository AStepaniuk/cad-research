add_library(cadgui_components OBJECT)

file(GLOB GUI_COMPONENTS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_components PRIVATE ${GUI_COMPONENTS_SOURCES})

target_include_directories(cadgui_components PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_components PUBLIC
    imgui_setup
)

target_link_libraries(cadgui_components PRIVATE
    corecad
    plan
)
