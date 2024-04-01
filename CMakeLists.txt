cmake_minimum_required(VERSION 3.25)
project(opengl_primary)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")                                           # 开启Debug宏

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE MAIN CONFIGURE_DEPENDS src/main.cpp src/Utility.cpp)
file(GLOB_RECURSE SRC_BASE CONFIGURE_DEPENDS src/base/*.h src/base/*.cpp)
file(GLOB_RECURSE SRC_CORE CONFIGURE_DEPENDS src/core/*.h src/core/*.cpp)
file(GLOB_RECURSE SRC_PRIMITIVE CONFIGURE_DEPENDS src/primitive/*.h src/primitive/*.cpp)
file(GLOB_RECURSE SRC_MATERIAL CONFIGURE_DEPENDS src/material/*.h src/material/*.cpp)
file(GLOB_RECURSE SRC_SCENE CONFIGURE_DEPENDS src/scene/*.h src/scene/*.cpp)
file(GLOB_RECURSE SRC_IMGUI CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp)
file(GLOB_RECURSE SRC_IMGUI_FILE_DIALOG CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/ImGuiFileDialog/*.cpp)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include/glad/glad.c ${SRC_BASE} ${SRC_PRIMITIVE} ${SRC_CORE} ${SRC_SCENE} ${SRC_MATERIAL} ${MAIN} )

link_directories(${CMAKE_SOURCE_DIR}/lib)
add_library(IMGUI ${SRC_IMGUI})
add_library(IMGUI_FILE_DIALOG ${SRC_IMGUI_FILE_DIALOG})

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/glfw3.dll)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libglog.dll)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libassimp-5.dll)
target_link_libraries(${PROJECT_NAME} IMGUI)
target_link_libraries(${PROJECT_NAME} IMGUI_FILE_DIALOG)

# 拷贝动态库
file(COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/lib/libglog.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/lib/libassimp-5.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/imgui.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})