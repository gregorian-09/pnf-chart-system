cmake_minimum_required(VERSION 3.31)

add_subdirectory(googletest)

enable_testing()

add_executable(ChartSystemTests
        test_pnf_box.cpp
        test_pnf_column.cpp
        test_pnf_chart.cpp
        test_pnf_trendline.cpp
        test_main.cpp

        ../sources/pnf_column.cpp
        ../sources/pnf_chart.cpp
        ../sources/pnf_chart_indicators.cpp
        test_pnf_moving_average.cpp
        test_pnf_bollinger_bands.cpp
        test_pnf_signal_detector.cpp
        test_pnf_pattern_recognizer.cpp
        test_pnf_support_resistance.cpp
        test_pnf_price_objective.cpp
)

target_include_directories(ChartSystemTests PRIVATE
        ${PROJECT_SOURCE_DIR}/headers
        ${PROJECT_SOURCE_DIR}/glfw/include
        googletest/googletest/include
)

target_link_libraries(ChartSystemTests
        PRIVATE OpenXLSX::OpenXLSX
        PRIVATE gtest
        PRIVATE gtest_main
)

include(GoogleTest)
gtest_discover_tests(ChartSystemTests)