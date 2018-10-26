cmake_minimum_required(VERSION 3.12)

project(opengl)

set(CMAKE_CXX_STANDARD 17)
set(DEP_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Dependencies)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
include(Dependencies.cmake)

# opengl
add_executable(opengl main.cpp Application.h TessellationApp.cpp TessellationApp.h OpenGl.cpp OpenGl.h Driver.cpp Driver.h OpenGlApiInfo.inl)
target_include_directories(opengl PUBLIC ${DEP_INSTALL_DIR}/include)

find_package(SDL2 REQUIRED)
target_include_directories(opengl PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(opengl ${SDL2_LIBRARIES})

find_package(OPENGL REQUIRED)
target_include_directories(opengl PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(opengl ${OPENGL_LIBRARY})

find_package(GLM REQUIRED)
target_include_directories(opengl PUBLIC ${GLM_INCLUDE_DIR})
target_link_libraries(opengl ${GLM_LIBRARY})

target_include_directories(opengl PUBLIC ${DEP_INSTALL_DIR}/include)
add_dependencies(opengl dep_spdlog dep_catch)