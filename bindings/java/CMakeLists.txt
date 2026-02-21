# CMake configuration for Java bindings.
cmake_minimum_required(VERSION 3.16)

find_package(JNI REQUIRED)

if(NOT JNI_FOUND)
    message(WARNING "JNI not found - Java bindings will not be built")
    return()
endif()

message(STATUS "Building Java bindings")
message(STATUS "  JNI Include Dirs: ${JNI_INCLUDE_DIRS}")
message(STATUS "  JNI Libraries: ${JNI_LIBRARIES}")

add_library(pnf_java SHARED pnf_java.cpp)

target_include_directories(pnf_java PRIVATE
    ${JNI_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../headers
)

if(TARGET pnf_shared)
    target_link_libraries(pnf_java PRIVATE pnf_shared)
elseif(TARGET pnf_static)
    target_link_libraries(pnf_java PRIVATE pnf_static)
else()
    message(FATAL_ERROR "Neither pnf_shared nor pnf_static target found")
endif()

set_target_properties(pnf_java PROPERTIES
    OUTPUT_NAME pnfjni
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

if(WIN32)
    set_target_properties(pnf_java PROPERTIES
        PREFIX ""
        SUFFIX ".dll"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
elseif(APPLE)
    set_target_properties(pnf_java PROPERTIES SUFFIX ".dylib")
else()
    set_target_properties(pnf_java PROPERTIES SUFFIX ".so")
endif()

install(TARGETS pnf_java
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
