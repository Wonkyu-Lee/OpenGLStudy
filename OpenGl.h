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

template<GLenum name> struct info {};
template<GLenum name> std::vector<typename info<name>::type> getv();
template<GLenum name> typename info<name>::type get();
template<GLenum name> std::string str();
#include "OpenGlApiInfo.inl"
#include "OpenGl.inl"

} // namespace gl

#endif //OPENGL_OPENGL_H
