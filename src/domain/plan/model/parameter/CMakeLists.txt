add_library(plan_model_parameter INTERFACE)

file(
    GLOB PARAMETER_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

target_sources(plan_model_parameter INTERFACE ${PARAMETER_HEADERS})

target_include_directories(plan_model_parameter INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(plan_model_parameter INTERFACE
    corecad
    plan_model_shape
)
