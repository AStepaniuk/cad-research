add_subdirectory(model)
add_subdirectory(util)
add_subdirectory(calculator)

add_library(
    corecad
    corecad.cpp
    ${CORE_MODEL_SOURCES}
    ${CORE_UTIL_SOURCES}
    ${CORE_CALCULATOR_SOURCES}
)

target_include_directories(
    corecad
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CORE_MODEL_INCLUDES}
    ${CORE_UTIL_INCLUDES}
    ${CORE_CALCULATOR_INCLUDES}
)
