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

    GLuint vertexShader = gl::compileShader(VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint fragmentShader = gl::compileShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);

    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    glDeleteShader(vertexShader);
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}