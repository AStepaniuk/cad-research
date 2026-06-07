add_subdirectory(parameter)
add_subdirectory(shape)

add_library(plan_model OBJECT)

file(GLOB MODEL_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(plan_model PRIVATE ${MODEL_SOURCES})

target_include_directories(plan_model_shape PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(plan_model PUBLIC
    corecad
    plan_model_parameter
    plan_model_shape
)

set(PLAN_MODEL_OBJECT_FILES
    $<TARGET_OBJECTS:plan_model>
    $<TARGET_OBJECTS:plan_model_parameter>
    $<TARGET_OBJECTS:plan_model_shape>
)

set(PLAN_MODEL_OBJECT_FILES ${PLAN_MODEL_OBJECT_FILES} PARENT_SCOPE)
