//
// Created by blazeq on 2018. 10. 12..
//

#include "TessellationApp.h"
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <catch2/catch.hpp>
#include "Driver.h"

using namespace std;

/****************************************************************************************************************
 * 체크리스트
 * - GL 세팅값 출력: (GL_MAX_PATCH_VERTICES, GL_PATCH_DEFAULT_OUTER_LEVEL, GL_PATCH_DEFAULT_INNER_LEVEL)
 * - spacing: (equal_spacing, fractional_even_spacing, fractional_odd_spacing)
 * - input primitive type: (triangles, quads, isolines), point_mode
 * - primitive generation order: (cw, ccw)
 ****************************************************************************************************************/

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

const GLchar* VERTEX_SHADER_F = R"(
#version 410 core
void main(void) {
    const vec4 vertices[3] = vec4[3](
        vec4(%f, %f, 0.5, 1.0),
        vec4(%f, %f, 0.5, 1.0),
        vec4(%f, %f, 0.5, 1.0)
    );
    gl_Position = vertices[gl_VertexID];
}
)";

const GLchar* getTriangleVertexShader() {
    char buf[1024];
    float r = 0.8f;
    sprintf(buf, VERTEX_SHADER_F,
            0.0f, r,
            (- r * cos(M_PI/6)), (- r * sin(M_PI/6)),
            (r * cos(M_PI/6)), (- r * sin(M_PI/6)));
    return buf;
}

const GLchar* TESS_CONTROL_SHADER = R"(
#version 410 core
layout (vertices = 3) out;
void main(void) {
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 0;
        gl_TessLevelOuter[0] = 1.0;
        gl_TessLevelOuter[1] = 1.0;
        gl_TessLevelOuter[2] = 1.0;
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
)";

const GLchar* TESS_CONTROL_SHADER_TRI_F = R"(
#version 410 core
layout (vertices = 3) out;
void main(void) {
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = %f;
        gl_TessLevelOuter[0] = %f;
        gl_TessLevelOuter[1] = %f;
        gl_TessLevelOuter[2] = %f;
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
)";

const GLchar* getTessCtlShader(float inner, float outer[3]) {
    char buf[1024];
    sprintf(buf, TESS_CONTROL_SHADER_TRI_F,
            inner, outer[0], outer[1], outer[2]);
    printf(buf);
    return buf;
}

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

void logProperties() {
    GLint maxPatchVertices;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    printf("GL_MAX_PATCH_VERTICES = %d\n", maxPatchVertices);

    GLfloat patchDefaultOuterLevels[4];
    glGetFloatv(GL_PATCH_DEFAULT_OUTER_LEVEL, patchDefaultOuterLevels);

    GLfloat patchDefaultInnerLevels[2];
    glGetFloatv(GL_PATCH_DEFAULT_INNER_LEVEL, patchDefaultInnerLevels);

    {
        assert(gl::get<GL_BLEND>() == GL_FALSE);
        assert(gl::get<GL_MAX_PATCH_VERTICES>() == 32);
        assert(gl::getv<GL_PATCH_DEFAULT_OUTER_LEVEL>() == vector<GLfloat>({1, 1, 1, 1}));
        assert(gl::getv<GL_PATCH_DEFAULT_INNER_LEVEL>() == vector<GLfloat>({1, 1}));

        cout << gl::str<GL_BLEND>() << endl;
        cout << gl::str<GL_MAX_PATCH_VERTICES>() << endl;
        cout << gl::str<GL_PATCH_DEFAULT_INNER_LEVEL>() << endl;
        cout << gl::str<GL_PATCH_DEFAULT_OUTER_LEVEL>() << endl;

        gl::flushErrors(cout);
    }
}

} // namespace

void TessellationApp::onAcquireContext() {
    logProperties();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPatchParameteri(GL_PATCH_VERTICES, 3); // same as default setting

    float outer[] = {1.0f, 1.0f, 1.0f};
    GLuint vertexShader = gl::compileShader(getTriangleVertexShader(), GL_VERTEX_SHADER);
    GLuint tessControlShader = gl::compileShader(getTessCtlShader(6.0f, outer), GL_TESS_CONTROL_SHADER);
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

TEST_CASE("Tessellation", "[tessellation]") {
    Driver::run<TessellationApp>("tessellation", 512, 512);
}