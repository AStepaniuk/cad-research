add_subdirectory(calculator_tests)

if(BUILD_TESTING)
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

    gtest_discover_tests(plan_tests DISCOVERY_MODE PRE_TEST)
    
endif()