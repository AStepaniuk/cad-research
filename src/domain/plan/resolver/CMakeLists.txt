add_library(plan_resolver OBJECT)

file(GLOB PLAN_RESOLVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources(plan_resolver PRIVATE ${PLAN_RESOLVER_SOURCES})

target_include_directories(plan_resolver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plan_resolver PRIVATE
    plan_model
)
