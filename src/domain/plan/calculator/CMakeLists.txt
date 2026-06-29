add_library(plan_calculator OBJECT)

file(GLOB PLAN_CALCULATOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(plan_calculator PRIVATE ${PLAN_CALCULATOR_SOURCES})

target_include_directories(plan_calculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plan_calculator PRIVATE
    corecad
    plan_model
    plan_resolver
)
