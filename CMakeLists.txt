cmake_minimum_required(VERSION 3.20)
project(FlyEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure Dependencies
# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/GLFW)

# ASSIMP
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
# Note: Assimp's CMake might need some tweaks or just work. 
# We point to the specific version folder.
add_subdirectory(Dependencies/ASSIMP/assimp-5.2.5)

# OpenGL
find_package(OpenGL REQUIRED)

if(WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# Projects
add_subdirectory(FlyEngine)
add_subdirectory(DevelopmentEnvironment)
add_subdirectory(Launcher)
