add_subdirectory(calculator_tests)

include(FetchContent)

# Download GoogleTest and GoogleMock
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make available
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

enable_testing()

add_executable(
    plan_tests
    ${PLAN_CALCULATOR_TESTS_SOURCES}
)

target_include_directories(
    plan_tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PLAN_CALCULATOR_TESTS_INCLUDES}
)

target_link_libraries(
    plan_tests
    PRIVATE
    gtest
    gmock
    gtest_main
    common_test_util
    corecad
    plan
)

add_test(NAME plan_tests COMMAND plan_tests)

gtest_discover_tests(plan_tests)
