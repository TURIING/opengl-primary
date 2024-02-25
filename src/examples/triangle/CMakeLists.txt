include_directories(.)

project(triangle)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS ./*.h ./*.cpp)
file(GLOB_RECURSE SRC_BASE CONFIGURE_DEPENDS ../../base/*.h ../../base/*.cpp)
file(GLOB_RECURSE SRC_PRIMITIVE CONFIGURE_DEPENDS ../../primitive/*.h ../../primitive/*.cpp)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include/glad/glad.c ${SRC_BASE} ${SRC_PRIMITIVE} ${SRC} )

link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/glfw3.dll)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libglog.dll)
target_link_libraries(${PROJECT_NAME} IMGUI)

## 拷贝动态库
file(COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/lib/libglog.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})