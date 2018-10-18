//
// Created by blazeq on 2018. 10. 16..
//

#ifndef OPENGL_OPENGL_H
#define OPENGL_OPENGL_H

#include <string>
#include <vector>
#include <iosfwd>
#include <SDL.h>
#include <OpenGL/gl3.h>
#include <functional>
#include <vector>
#include <sstream>

namespace gl {

const std::string& strErrorCode(GLenum errorCode);
const std::string& strShaderType(GLenum shaderType);
void flushErrors(std::ostream& out);
std::vector<GLenum> collectErrors();

GLuint compileShader(const GLchar* source, GLenum type);

template<GLenum name>
struct info {};

template<>
struct info<GL_BLEND> {
    static const char* str() { return "GL_BLEND"; };
    using type = GLboolean;
    enum { paramCount = 1 };
};

template<>
struct info<GL_MAX_PATCH_VERTICES> {
    static const char* str() { return "GL_MAX_PATCH_VERTICES"; };
    using type = GLint;
    enum { paramCount = 1 };
};

template<>
struct info<GL_POINT_SIZE> {
    static const char* str() { return "GL_POINT_SIZE"; };
    using type = GLint;
    enum { paramCount = 1 };
};

template<>
struct info<GL_PATCH_DEFAULT_INNER_LEVEL> {
    static const char* str() { return "GL_PATCH_DEFAULT_OUTER_LEVEL"; };
    using type = GLfloat;
    enum { paramCount = 2 };
};

template<>
struct info<GL_PATCH_DEFAULT_OUTER_LEVEL> {
    static const char* str() { return "GL_PATCH_DEFAULT_OUTER_LEVEL"; };
    using type = GLfloat;
    enum { paramCount = 4 };
};

template<GLenum name>
std::vector<typename info<name>::type> getv() {
    using value_type = typename info<name>::type;

    std::vector<value_type> values;
    values.resize(info<name>::paramCount);
    value_type* pos = values.data();
    if (std::is_same<value_type, GLint>::value) {
        glGetIntegerv(name, (GLint*)pos);
    } else if (std::is_same<value_type, GLfloat>::value) {
        glGetFloatv(name, (GLfloat*)pos);
    } else if (std::is_same<value_type, GLboolean>::value) {
        glGetBooleanv(name, (GLboolean*)pos);
    }

    return values;
}

template<GLenum name>
typename info<name>::type get() {
    using value_type = typename info<name>::type;
    static_assert(info<name>::paramCount == 1);
    return getv<name>().front();
}

template<GLenum name>
std::string str() {
    auto values = getv<name>();

    std::stringstream out;
    out << info<name>::str();

    bool isBoolType =std::is_same<typename info<name>::type, GLboolean>::value;
    if (isBoolType) {
        for (int i = 0; i < values.size(); ++i) {
            auto strVal = values[i] ? "GL_TRUE" : "GL_FALSE";
            out << (i == 0 ? "{" : ", ") << strVal;
        }
    } else {
        for (int i = 0; i < values.size(); ++i) {
            out << (i == 0 ? "{" : ", ") << values[i];
        }
    }

    out << "}";

    return out.str();
}

} // namespace gl

#endif //OPENGL_OPENGL_H
