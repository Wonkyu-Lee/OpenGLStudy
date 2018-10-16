//
// Created by blazeq on 2018. 10. 16..
//

#include "OpenGl.h"
#include <iostream>
#include <unordered_map>

namespace gl {

using namespace std;

const std::string& strErrorCode(GLenum errorCode) {
    static const string UNKNOWN = "Unknown";
    static const unordered_map<GLenum, string> MAP = {
            { GL_INVALID_ENUM, "GL_INVALID_ENUM" },
            { GL_INVALID_VALUE, "GL_INVALID_VALUE" },
            { GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
            { GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION" },
            { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" },
    };

    auto found = MAP.find(errorCode);
    return (found != MAP.end() ? found->second : UNKNOWN);
}

const std::string& strShaderType(GLenum shaderType) {
    static const string UNKNOWN = "Unknown";
    static const unordered_map<GLenum, string> MAP = {
            { GL_VERTEX_SHADER, "GL_VERTEX_SHADER" },
            { GL_TESS_CONTROL_SHADER, "GL_TESS_CONTROL_SHADER" },
            { GL_TESS_EVALUATION_SHADER, "GL_TESS_EVALUATION_SHADER" },
            { GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER" },
            { GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER" },
    };

    auto found = MAP.find(shaderType);
    return (found != MAP.end() ? found->second : UNKNOWN);
}

void flushErrors(ostream& out) {
    vector<GLenum> errors = collectErrors();
    if (errors.empty())
        return;

    for (auto each : errors) {
        out  << "[gl error] = " << strErrorCode(each) << endl;
    }
}

vector<GLenum> collectErrors() {
    vector<GLenum> errors;
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        errors.push_back(error);
    }
    return errors;
}

GLuint compileShader(const GLchar* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_TRUE)
        return shader;

    enum { BUFFER_SIZE = 4096 };
    GLsizei length;
    GLchar infoLog[BUFFER_SIZE];
    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, infoLog);
    cerr << "[glsl error:" << strShaderType(type) << "]" << infoLog << endl;
    return 0u;
}

} // namespace gl