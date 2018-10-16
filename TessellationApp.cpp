//
// Created by blazeq on 2018. 10. 12..
//

#include "TessellationApp.h"
#include <iostream>

using namespace std;

namespace {

const GLchar* VERTEX_SHADER[] = {R"(
#version 410 core
void main(void) {
    const vec4 vertices[3] = vec4[3](
        vec4(0.25, -0.25, 0.5, 1.0),
        vec4(-0.25, -0.25, 0.5, 1.0),
        vec4(0.25, 0.25, 0.5, 1.0)
    );
    gl_Position = vertices[gl_VertexID];
}
)"};

const GLchar* FRAGMENT_SHADER[] = {R"(
#version 410 core
out vec4 color
void main(void) {
    color = vec4(0.0, 0.8, 1.0, 1.0);
}
)"};

void checkGlError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        cout << "error = 0x" << hex << error << endl;
    }
}

} // namespace

Application* Application::create() {
    return new TessellationApp("tessellation", 512, 512);
}

void TessellationApp::onAcquireContext() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(40.0f);

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, VERTEX_SHADER, nullptr);
    glCompileShader(vertexShader);
    checkGlError();

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, FRAGMENT_SHADER, nullptr);
    glCompileShader(fragmentShader);
    checkGlError();

    _program = glCreateProgram();
    checkGlError();
    glAttachShader(_program, vertexShader);
    checkGlError();
    glAttachShader(_program, fragmentShader);
    checkGlError();
    glLinkProgram(_program);
    checkGlError();

    glDeleteShader(vertexShader);
    checkGlError();

    glDeleteShader(fragmentShader);
    checkGlError();

    glGenVertexArrays(1, &_vao);
    checkGlError();
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
    checkGlError();

    glUseProgram(_program);
    checkGlError();

    glBindVertexArray(_vao);
    checkGlError();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError();
}