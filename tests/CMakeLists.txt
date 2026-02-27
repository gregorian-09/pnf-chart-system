enable_testing()

find_package(GTest CONFIG QUIET)

if(NOT GTest_FOUND)
    set(GTEST_SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/googletest")
    if(EXISTS "${GTEST_SOURCE_DIR}/CMakeLists.txt")
        message(STATUS "Using bundled GoogleTest at ${GTEST_SOURCE_DIR}")
        add_subdirectory("${GTEST_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/_deps/googletest-build")
    else()
        include(FetchContent)
        FetchContent_Declare(
                googletest
                URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
endif()

add_executable(pnf_tests
        test_box.cpp
        test_column.cpp
        test_chart.cpp
        test_indicators.cpp
        test_trendline.cpp
        test_visualization.cpp
        test_c_api.cpp
)

if(PNF_BUILD_SHARED)
    target_link_libraries(pnf_tests
            PRIVATE pnf::shared
            PRIVATE GTest::gtest
            PRIVATE GTest::gtest_main
    )

    if(WIN32)
        add_custom_command(TARGET pnf_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:pnf_shared>
                $<TARGET_FILE_DIR:pnf_tests>
                COMMENT "Copying pnf_shared DLL to test directory"
        )
    endif()

    if(UNIX AND NOT APPLE)
        set_target_properties(pnf_tests PROPERTIES
                BUILD_RPATH "${CMAKE_BINARY_DIR}/lib"
                INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
        )
    endif()

    if(APPLE)
        set_target_properties(pnf_tests PROPERTIES
                BUILD_RPATH "${CMAKE_BINARY_DIR}/lib"
                INSTALL_RPATH "@loader_path/../lib"
        )
    endif()
else()
    target_link_libraries(pnf_tests
            PRIVATE pnf::static
            PRIVATE GTest::gtest
            PRIVATE GTest::gtest_main
    )
endif()

target_include_directories(pnf_tests
        PRIVATE ${CMAKE_SOURCE_DIR}/headers
)

include(GoogleTest)

gtest_discover_tests(pnf_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
