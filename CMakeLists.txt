cmake_minimum_required(VERSION 3.23.0)

project(Cord)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/cord/glad.c
)

add_executable(cord_test ${SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(sub/glfw)

target_link_libraries(cord_test glfw)

target_include_directories(cord_test
    PUBLIC sub/glfw/include
    PUBLIC glad
)

target_link_directories(cord_test
    PRIVATE sub/glfw/src
)