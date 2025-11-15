add_subdirectory(model)
add_subdirectory(generator)
add_subdirectory(calculator)

add_library(
    plan
    ${PLAN_MODEL_SOURCES}
    ${PLAN_GENERATOR_SOURCES}
    ${PLAN_CALCULATOR_SOURCES}
)

target_include_directories(
    plan
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PLAN_MODEL_INCLUDES}
    ${PLAN_GENERATOR_INCLUDES}
    ${PLAN_CALCULATOR_INCLUDES}
)

target_link_libraries(plan PRIVATE corecad)
