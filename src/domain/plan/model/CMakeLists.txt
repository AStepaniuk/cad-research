add_subdirectory(parameter)
add_subdirectory(shape)

add_library(plan_model INTERFACE)
target_link_libraries(plan_model INTERFACE 
    plan_model_parameter
    plan_model_shape
)

set(PLAN_MODEL_OBJECT_FILES
    $<TARGET_OBJECTS:plan_model_parameter>
    $<TARGET_OBJECTS:plan_model_shape>
)

set(PLAN_MODEL_OBJECT_FILES ${PLAN_MODEL_OBJECT_FILES} PARENT_SCOPE)
