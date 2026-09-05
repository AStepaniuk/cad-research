add_library(cadgui_cmd_parser OBJECT)

file(GLOB CMD_PARSER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(cadgui_cmd_parser PRIVATE ${CMD_PARSER_SOURCES})

target_include_directories(cadgui_cmd_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

