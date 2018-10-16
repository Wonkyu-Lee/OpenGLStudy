//
// Created by blazeq on 2018. 10. 16..
//

#include "OpenGl.h"
#include <iostream>
#include <unordered_map>

namespace gl {

using namespace std;

vector<GLenum> collectErrors() {
    vector<GLenum> errors;
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        errors.push_back(error);
    }
    return errors;
}

const char* getErrorMessage(GLenum error) {
    static const string UNKNOWN_ERROR = "Unknown error!";
    static const unordered_map<GLenum, string> ERROR_CODES = {
        { GL_INVALID_ENUM, "GL_INVALID_ENUM" },
        { GL_INVALID_VALUE, "GL_INVALID_VALUE" },
        { GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
        { GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION" },
        { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" },
    };

    auto found = ERROR_CODES.find(error);
    return (found != ERROR_CODES.end() ? found->second : UNKNOWN_ERROR).c_str();
}

void flushErrors(ostream& out) {
    vector<GLenum> errors = collectErrors();
    if (errors.empty())
        return;

    for (auto each : errors) {
        out  << "[gl error] = " << getErrorMessage(each) << endl;
    }
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
    cerr << infoLog << endl;
    return 0u;
}

} // namespace gl