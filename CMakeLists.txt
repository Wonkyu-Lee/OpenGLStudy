cmake_minimum_required(VERSION 3.12)
project(opengl)

set(CMAKE_CXX_STANDARD 17)
add_executable(opengl Driver.cpp Application.h TessellationApp.cpp TessellationApp.h OpenGl.cpp OpenGl.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(opengl ${SDL2_LIBRARIES})

find_package(OPENGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(opengl ${OPENGL_LIBRARY})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})
target_link_libraries(opengl ${GLM_LIBRARY})