project(system)

set(SOURCE_FILES
    Application.h
    OpenGl.cpp OpenGl.h OpenGl.inl OpenGlApiInfo.inl
    Driver.cpp Driver.h
)

add_library(system ${SOURCE_FILES})

target_include_directories(system PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(system PUBLIC ${OPENGL_INCLUDE_DIR})