cmake_minimum_required(VERSION 3.25)
project(opengl_primary)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SRC_IMGUI CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp)

add_library(IMGUI ${SRC_IMGUI})

add_subdirectory(src/examples/triangle)
add_subdirectory(src/examples/cube)
add_subdirectory(src/examples/model_load)
