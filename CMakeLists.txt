cmake_minimum_required(VERSION 3.5)

project(wcap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE _UNICODE _WINDOWS)

set(MY_BINARY_DIR ${CMAKE_BINARY_DIR}/bin-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MY_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MY_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MY_BINARY_DIR})

add_library(wcapdll SHARED dllmain.cpp wcap.h)
target_link_libraries(wcapdll d3d9)

add_executable(wcap main.cpp dx9offsets.cpp dummy_window.cpp inject.h dxoffsets.h dummy_window.h wcap.h)
target_link_libraries(wcap d3d9)
