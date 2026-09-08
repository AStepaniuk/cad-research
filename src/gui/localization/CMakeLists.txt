add_library(cadgui_localization OBJECT)

file(GLOB LOCALIZATION_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_localization PRIVATE ${LOCALIZATION_SOURCES})

target_include_directories(cadgui_localization PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
