add_library(plan_model_shape OBJECT)

file(GLOB SHAPE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(plan_model_shape PRIVATE ${SHAPE_SOURCES})

target_include_directories(plan_model_shape PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(plan_model_shape PRIVATE corecad)
