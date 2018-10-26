cmake_minimum_required(VERSION 3.12)

project(opengl)

set(CMAKE_CXX_STANDARD 17)
set(DEP_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Dependencies)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

include(Dependencies.cmake)

find_package(SDL2 REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM REQUIRED)

add_subdirectory(system)
add_subdirectory(examples)

add_dependencies(system dep_spdlog dep_catch)
add_dependencies(examples system)