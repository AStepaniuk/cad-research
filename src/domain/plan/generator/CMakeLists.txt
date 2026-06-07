add_library(plan_generator OBJECT)

file(GLOB PLAN_GENERATOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(plan_generator PRIVATE ${PLAN_GENERATOR_SOURCES})

target_include_directories(plan_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plan_generator PRIVATE
    corecad
    plan_model
)
