cmake_minimum_required(VERSION 3.10)
project(PlaneGCS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLANEGCS_SOURCES
    GCS.cpp
    SubSystem.cpp
    Constraints.cpp
    Geo.cpp
    qp_eq.cpp
)

# Create the dynamic/shared library
add_library(planegcs SHARED ${PLANEGCS_SOURCES})

target_compile_definitions(planegcs PRIVATE BUILDING_PLANEGCS)
target_include_directories(planegcs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost REQUIRED)
target_link_libraries(planegcs PRIVATE Boost::boost)

find_package(Eigen3 REQUIRED)
target_link_libraries(planegcs PUBLIC Eigen3::Eigen)

set_target_properties(planegcs PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)
