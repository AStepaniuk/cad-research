add_library(cadgui_doc OBJECT)

file(GLOB DOC_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_doc PRIVATE ${DOC_SOURCES})

target_include_directories(cadgui_doc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cadgui_doc PRIVATE
    corecad
    plan
)
