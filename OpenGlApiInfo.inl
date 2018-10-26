//
// Created by blazeq on 27/10/2018.
//

#define GL_API_INFO(_name, _type, _paramCount) \
template<> struct info<_name> { \
    static const char* str() { return #_name; }; \
    using type = _type; \
    enum { paramCount = _paramCount }; \
}

GL_API_INFO(GL_BLEND, GLboolean, 1);
GL_API_INFO(GL_MAX_PATCH_VERTICES, GLint, 1);
GL_API_INFO(GL_POINT_SIZE, GLint, 1);
GL_API_INFO(GL_PATCH_DEFAULT_INNER_LEVEL, GLfloat, 2);
GL_API_INFO(GL_PATCH_DEFAULT_OUTER_LEVEL, GLfloat, 4);
