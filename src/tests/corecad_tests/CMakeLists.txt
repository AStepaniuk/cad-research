add_subdirectory(model_tests)

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
        corecad_tests
        ${CORECAD_MODEL_TESTS_SOURCES}
    )

    target_include_directories(
        corecad_tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CORECAD_MODEL_TESTS_INCLUDES}
    )

    target_link_libraries(
        corecad_tests
        PRIVATE
        gtest
        gmock
        gtest_main
        common_test_util
        corecad
    )

    gtest_discover_tests(corecad_tests DISCOVERY_MODE PRE_TEST)
    
endif()