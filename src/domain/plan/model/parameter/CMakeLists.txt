add_library(plan_model_parameter OBJECT)

file(GLOB PARAMETER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(plan_model_parameter PRIVATE ${PARAMETER_SOURCES})

target_include_directories(plan_model_parameter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(plan_model_parameter PRIVATE
    corecad
    plan_model_shape
)
