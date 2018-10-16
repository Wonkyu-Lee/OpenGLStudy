//
// Created by blazeq on 2018. 10. 12..
//

#include "TessellationApp.h"
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

namespace {

const GLchar* VERTEX_SHADER = R"(
#version 410 core
void main(void) {
    const vec4 vertices[3] = vec4[3](
        vec4(0.25, -0.25, 0.5, 1.0),
        vec4(-0.25, -0.25, 0.5, 1.0),
        vec4(0.25, 0.25, 0.5, 1.0)
    );
    gl_Position = vertices[gl_VertexID];
}
)";

const GLchar* TESS_CONTROL_SHADER = R"(
#version 410 core
layout (vertices = 3) out;
void main(void) {
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 5.0;
        gl_TessLevelOuter[0] = 5.0;
        gl_TessLevelOuter[1] = 5.0;
        gl_TessLevelOuter[2] = 5.0;
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
)";

const GLchar* TESS_EVALUATION_SHADER = R"(
#version 410 core
layout (triangles, equal_spacing, cw) in;
void main(void) {
    gl_Position =
        (gl_TessCoord.x * gl_in[0].gl_Position) +
        (gl_TessCoord.y * gl_in[1].gl_Position) +
        (gl_TessCoord.z * gl_in[2].gl_Position);
}
)";

const GLchar* FRAGMENT_SHADER = R"(
#version 410 core
out vec4 color;
void main(void) {
    color = vec4(0.0, 0.8, 1.0, 1.0);
}
)";

} // namespace

Application* Application::create() {
    return new TessellationApp("tessellation", 512, 512);
}

void TessellationApp::onAcquireContext() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // glPatchParameteri(GL_PATCH_VERTICES, 3);

    GLuint vertexShader = gl::compileShader(VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint tessControlShader = gl::compileShader(TESS_CONTROL_SHADER, GL_TESS_CONTROL_SHADER);
    GLuint tessEvaluationShader = gl::compileShader(TESS_EVALUATION_SHADER, GL_TESS_EVALUATION_SHADER);
    GLuint fragmentShader = gl::compileShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, tessControlShader);
    glAttachShader(_program, tessEvaluationShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);
    gl::flushErrors(cerr);

    glDeleteShader(vertexShader);
    glDeleteShader(tessControlShader);
    glDeleteShader(tessEvaluationShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &_vao);
    gl::flushErrors(cerr);
}

void TessellationApp::onReleaseContext() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteProgram(_program);
}

void TessellationApp::onResize(int width, int height) {
    glViewport(0, 0, width, height);
}

bool TessellationApp::onUpdate() {
    SDL_Event event;
    bool quit = false;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            quit = true;

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_r:
                    glClearColor(1.0, 0.0, 0.0, 1.0);
                    break;
                case SDLK_g:
                    glClearColor(0.0, 1.0, 0.0, 1.0);
                    break;
                case SDLK_b:
                    glClearColor(0.0, 0.0, 1.0, 1.0);
                    break;
                default:
                    break;
            }
        }
    }

    return quit;
}

void TessellationApp::onDraw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_program);
    glBindVertexArray(_vao);
    glDrawArrays(GL_PATCHES, 0, 3);
    gl::flushErrors(cerr);
}