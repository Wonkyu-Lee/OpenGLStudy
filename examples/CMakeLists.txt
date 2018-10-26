project(examples)

set(SOURCE_FILES
    main.cpp
    TessellationApp.cpp TessellationApp.h
)

add_executable(examples ${SOURCE_FILES})

target_include_directories(examples PUBLIC ../system)
target_include_directories(examples PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(examples PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(examples PUBLIC ${GLM_INCLUDE_DIR})
target_include_directories(examples PUBLIC ${DEP_INSTALL_DIR}/include)

target_link_libraries(examples ${SDL2_LIBRARIES})
target_link_libraries(examples ${OPENGL_LIBRARY})
target_link_libraries(examples ${GLM_LIBRARY})
target_link_libraries(examples system)