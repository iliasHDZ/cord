cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Cord)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/cord/glad.c
)

add_executable(cord_test ${SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

add_subdirectory(sub/assimp)
add_subdirectory(sub/glfw)
add_subdirectory(sub/glm)

target_link_libraries(cord_test glfw glm assimp)

target_include_directories(cord_test
    PUBLIC sub/glfw/include
    PUBLIC sub/assimp/include
    PUBLIC glad
)

target_link_directories(cord_test
    PRIVATE sub/glfw/src
)
