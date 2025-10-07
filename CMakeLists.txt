cmake_minimum_required(VERSION 3.31)
project(ChartSystem)

set(CMAKE_CXX_STANDARD 20)

set(OPENXLSX_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

add_subdirectory(OpenXLSX)
add_subdirectory(glfw)
add_subdirectory(test)
add_subdirectory(examples)

add_library(ChartSystemCore STATIC
        sources/pnf_column.cpp
        sources/pnf_chart.cpp
        sources/pnf_chart_indicators.cpp
        sources/csv_loader.cpp
)

target_include_directories(ChartSystemCore PUBLIC
        ${PROJECT_SOURCE_DIR}/headers
        ${PROJECT_SOURCE_DIR}/glfw/include
)

target_link_libraries(ChartSystemCore
        PUBLIC OpenXLSX::OpenXLSX
        PUBLIC glfw
)

add_executable(ChartSystem
        sources/main.cpp
        sources/pnf_chart_visualizer.cpp

        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_tables.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp

        implot/implot.cpp
        implot/implot_items.cpp
)

include_directories(
        ${PROJECT_SOURCE_DIR}/headers
        ${PROJECT_SOURCE_DIR}/imgui
        ${PROJECT_SOURCE_DIR}/imgui/backends
        ${PROJECT_SOURCE_DIR}/implot
        ${PROJECT_SOURCE_DIR}/glfw/include
)

target_link_libraries(ChartSystem
        PRIVATE ChartSystemExamples
        PRIVATE ChartSystemCore
        PRIVATE OpenGL::GL
)