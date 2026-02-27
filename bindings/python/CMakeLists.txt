# CMake configuration for Python bindings.

set(Python3_FIND_REGISTRY LAST)
set(Python3_FIND_STRATEGY LOCATION)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

find_package(pybind11 CONFIG REQUIRED)

if(NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            pybind11
            GIT_REPOSITORY https://github.com/pybind/pybind11.git
            GIT_TAG v3.0.1
    )
    FetchContent_MakeAvailable(pybind11)
endif()

pybind11_add_module(pypnf pnf_python.cpp)

target_link_libraries(pypnf PRIVATE
        pnf_static
        Python3::Python
)

target_include_directories(pypnf PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(pypnf PROPERTIES
        OUTPUT_NAME "pypnf"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/python
)

install(TARGETS pypnf DESTINATION ${CMAKE_INSTALL_LIBDIR}/python)
