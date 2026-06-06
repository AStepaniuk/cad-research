file(GLOB_RECURSE GUI_VIEW_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

set(GUI_VIEW_SOURCES ${GUI_VIEW_SOURCES} PARENT_SCOPE)

set(GUI_VIEW_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)



add_library(cadgui_view OBJECT)

file(GLOB VIEW_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_view PRIVATE ${VIEW_SOURCES})

target_include_directories(cadgui_view PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_view PUBLIC
    corecad
    plan
    imgui_setup
)

target_link_libraries(cadgui_view PRIVATE
    cadgui_doc
)
