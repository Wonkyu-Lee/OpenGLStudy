//
// Created by blazeq on 2018. 10. 12..
//

#include "TessellationApp.h"
#include <iostream>
#include <sstream>

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

void assertGlValid() {
    GLenum error = glGetError();
    if (error == GL_NO_ERROR)
        return;

    string msg;
    switch (error) {
        case GL_INVALID_ENUM:
            msg = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            msg = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            msg = "GL_INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            msg = "GL_OUT_OF_MEMORY";
            break;
        default:
            msg = "Unknown error!";
    }

    stringstream out;
    out  << "[gl error] = " << msg << endl;
    cerr << out.str();

    assert(false);
}

GLuint compileShader(const GLchar* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_TRUE)
        return shader;

    const GLsizei BUFFER_SIZE = 4096;
    GLsizei length;
    GLchar infoLog[BUFFER_SIZE];
    glGetShaderInfoLog(shader, 4096, &length, infoLog);
    cerr << infoLog << endl;
    return 0u;
}

} // namespace

Application* Application::create() {
    return new TessellationApp("tessellation", 512, 512);
}

void TessellationApp::onAcquireContext() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(40.0f);

    GLuint vertexShader = compileShader(VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &_vao);

    assertGlValid();
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