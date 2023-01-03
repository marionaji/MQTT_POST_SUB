option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")
if (ENABLE_UNIT_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
    
    enable_testing()

    set(CS_COMM_TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test) 
    add_executable(
        pb_test
        ${CS_COMM_TEST_SRC}/mqtt_pb_test.cpp
        src/pb_parse.cpp
        ${PROTO_HEADER}   
        ${PROTO_SRC}
    )
    target_include_directories(
        pb_test 
        PRIVATE
        include
    )
    target_link_libraries(
        pb_test
        gtest_main
    )
    gtest_discover_tests(pb_test)

 endif()