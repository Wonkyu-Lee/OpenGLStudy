//
// Created by blazeq on 2018. 10. 12..
//

#include "TessellationApp.h"
#include <iostream>
#include <vector>

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
out vec4 color;
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

    auto compileShader = [](const GLchar** shaderSourceCode, bool isVertexShader) {
        auto shader = glCreateShader(isVertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, shaderSourceCode, nullptr);
        glCompileShader(shader);

        GLint compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        if (!compileStatus) {
            const GLsizei LOG_MAX_LENGTH = 4096;
            vector<GLchar> compileLog;
            compileLog.resize(LOG_MAX_LENGTH);
            GLsizei compileLogLength;
            glGetShaderInfoLog(shader, LOG_MAX_LENGTH, &compileLogLength, compileLog.data());
            cout << "log length: " << compileLogLength << endl;
            cerr << (isVertexShader ? "vertex" : "fragment") << " shader compile error: " << compileLog.data() << endl;
            return 0u;
        }
        
        return shader;
    };

    auto vertexShader = compileShader(VERTEX_SHADER, true);
    auto fragmentShader = compileShader(FRAGMENT_SHADER, false);

    if (!vertexShader || !fragmentShader) {
        cerr << "failed to compile shader. please refer the error log above." << endl;
        _program = 0;
        return;
    }

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

    if (!_program)
        return;
    glUseProgram(_program);
    checkGlError();

    glBindVertexArray(_vao);
    checkGlError();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError();
}