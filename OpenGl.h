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

namespace gl {

std::vector<GLenum> collectErrors();
const char* getErrorMessage(GLenum error);
void flushErrors(std::ostream& out);
GLuint compileShader(const GLchar* source, GLenum type);

} // namespace gl

#endif //OPENGL_OPENGL_H
