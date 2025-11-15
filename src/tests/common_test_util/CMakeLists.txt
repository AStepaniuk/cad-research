add_subdirectory(assertions)

include(FetchContent)

# Download GoogleTest and GoogleMock
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

# Make available
FetchContent_MakeAvailable(googletest)

add_library(
    common_test_util
    ${ASSERTIONS_SOURCES}
)

target_include_directories(
    common_test_util
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ASSERTIONS_INCLUDES}
)

target_link_libraries(
    common_test_util
    PRIVATE
    gtest
    gmock
    gtest_main
    corecad
)
