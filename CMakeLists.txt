cmake_minimum_required(VERSION 3.28)
project(pnf VERSION 0.1.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(PNF_BUILD_TESTS "Build unit tests" ON)
option(PNF_BUILD_EXAMPLES "Build examples" ON)
option(PNF_BUILD_PYTHON "Build Python bindings" ON)
option(PNF_BUILD_JAVA "Build Java bindings (JNI)" ON)
option(PNF_BUILD_RUST "Build Rust bindings" ON)
option(PNF_BUILD_SHARED "Build shared library" ON)
option(PNF_BUILD_STATIC "Build static library" ON)
option(PNF_BUILD_VIEWER "Build interactive viewer (requires SDL2)" ON)

include(GNUInstallDirs)
include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PNF_SOURCES
        sources/pnf/box.cpp
        sources/pnf/column.cpp
        sources/pnf/csv_loader.cpp
        sources/pnf/trendline.cpp
        sources/pnf/indicators.cpp
        sources/pnf/chart.cpp
        sources/pnf/types.cpp
        sources/pnf/viewer.cpp
        sources/pnf/visualization.cpp
)

set(PNF_HEADERS
        headers/pnf/pnf.hpp
        headers/pnf/version.hpp
        headers/pnf/types.hpp
        headers/pnf/box.hpp
        headers/pnf/column.hpp
        headers/pnf/trendline.hpp
        headers/pnf/chart.hpp
        headers/pnf/indicators.hpp
        headers/pnf/visualization.hpp
        headers/pnf/viewer.hpp
        headers/pnf/csv_loader.hpp
)

if(PNF_BUILD_VIEWER)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    if(SDL2_FOUND AND SDL2_ttf_FOUND)
        set(PNF_HAS_SDL2 TRUE)
        message(STATUS "SDL2 found - interactive viewer enabled")
    else()
        set(PNF_HAS_SDL2 FALSE)
        message(STATUS "SDL2 not found - interactive viewer disabled")
    endif()
endif()

set(PNF_C_API_SOURCES
        bindings/c/pnf_c.cpp
)

if(PNF_BUILD_STATIC)
    add_library(pnf_static STATIC ${PNF_SOURCES})
    target_include_directories(pnf_static PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headers>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    if(PNF_HAS_SDL2)
        target_compile_definitions(pnf_static PRIVATE PNF_HAS_SDL2)
        target_link_libraries(pnf_static PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)
    endif()

    if(WIN32 AND PNF_BUILD_SHARED)
        set_target_properties(pnf_static PROPERTIES
                OUTPUT_NAME pnf_static
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
        )
    else()
        set_target_properties(pnf_static PROPERTIES
                OUTPUT_NAME pnf
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
        )
    endif()

    add_library(pnf::static ALIAS pnf_static)
endif()

if(PNF_BUILD_SHARED)
    add_library(pnf_shared SHARED ${PNF_SOURCES} ${PNF_C_API_SOURCES})
    target_include_directories(pnf_shared PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headers>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bindings/c>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    target_compile_definitions(pnf_shared PRIVATE PNF_BUILD_DLL)
    if(PNF_HAS_SDL2)
        target_compile_definitions(pnf_shared PRIVATE PNF_HAS_SDL2)
        target_link_libraries(pnf_shared PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)
    endif()

    set_target_properties(pnf_shared PROPERTIES
            OUTPUT_NAME pnf
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    )

    if(WIN32)
        set_target_properties(pnf_shared PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    endif()

    if(UNIX AND NOT APPLE)
        set_target_properties(pnf_shared PROPERTIES
                INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
                BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        )
    endif()

    if(APPLE)
        set_target_properties(pnf_shared PROPERTIES
                INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
                BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
                INSTALL_RPATH "@loader_path"
        )
    endif()

    add_library(pnf::shared ALIAS pnf_shared)
endif()

add_library(pnf INTERFACE)
if(PNF_BUILD_SHARED)
    target_link_libraries(pnf INTERFACE pnf_shared)
elseif(PNF_BUILD_STATIC)
    target_link_libraries(pnf INTERFACE pnf_static)
endif()
add_library(pnf::pnf ALIAS pnf)

if(PNF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PNF_BUILD_EXAMPLES)
    add_subdirectory(examples/cpp)
endif()

if(PNF_BUILD_PYTHON)
    add_subdirectory(bindings/python)
endif()

if(PNF_BUILD_JAVA)
    add_subdirectory(bindings/java)
endif()

if(PNF_BUILD_RUST)
    add_subdirectory(bindings/rust)
endif()

install(DIRECTORY headers/pnf DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES bindings/c/pnf_c.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pnf)

if(PNF_BUILD_STATIC)
    install(TARGETS pnf_static EXPORT pnfTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

if(PNF_BUILD_SHARED)
    install(TARGETS pnf_shared EXPORT pnfTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

install(EXPORT pnfTargets
        FILE pnfTargets.cmake
        NAMESPACE pnf::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pnf
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pnfConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_file(cmake/pnfConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/pnfConfig.cmake"
        @ONLY
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pnfConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/pnfConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pnf
)

message(STATUS "")
message(STATUS "PnF Chart Library v${PROJECT_VERSION}")
message(STATUS "  Build tests:    ${PNF_BUILD_TESTS}")
message(STATUS "  Build examples: ${PNF_BUILD_EXAMPLES}")
message(STATUS "  Build Python:   ${PNF_BUILD_PYTHON}")
message(STATUS "  Build Java:     ${PNF_BUILD_JAVA}")
message(STATUS "  Build Rust:     ${PNF_BUILD_RUST}")
message(STATUS "  Build shared:   ${PNF_BUILD_SHARED}")
message(STATUS "  Build static:   ${PNF_BUILD_STATIC}")
message(STATUS "  Build viewer:   ${PNF_BUILD_VIEWER} (SDL2: ${PNF_HAS_SDL2})")
message(STATUS "")
