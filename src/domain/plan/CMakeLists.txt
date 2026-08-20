add_subdirectory(model)
add_subdirectory(generator)
add_subdirectory(calculator)

add_library(plan STATIC)

target_sources(plan PRIVATE
    ${PLAN_MODEL_OBJECT_FILES}
    $<TARGET_OBJECTS:plan_generator>
    $<TARGET_OBJECTS:plan_calculator>
)

target_include_directories(plan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plan PRIVATE corecad)
target_link_libraries(plan PUBLIC 
    plan_model
    plan_generator
    plan_calculator
)
