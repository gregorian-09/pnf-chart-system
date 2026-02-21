# CMake configuration for Rust bindings.
cmake_minimum_required(VERSION 3.16)

find_program(CARGO cargo)

if(NOT CARGO)
    message(WARNING "Cargo not found - Rust bindings will not be built")
    return()
endif()

message(STATUS "Building Rust bindings")
message(STATUS "  Cargo: ${CARGO}")

set(RUST_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/target)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CARGO_BUILD_TYPE --release)
    set(RUST_OUTPUT_DIR ${RUST_TARGET_DIR}/release)
else()
    set(CARGO_BUILD_TYPE)
    set(RUST_OUTPUT_DIR ${RUST_TARGET_DIR}/debug)
endif()

add_custom_target(pnf_rust ALL
    COMMAND ${CARGO} build ${CARGO_BUILD_TYPE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building Rust bindings"
    DEPENDS pnf_shared
)

add_custom_target(pnf_rust_test
    COMMAND ${CARGO} test ${CARGO_BUILD_TYPE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing Rust bindings"
    DEPENDS pnf_rust
)
