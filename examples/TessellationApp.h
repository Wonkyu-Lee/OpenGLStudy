//
// Created by blazeq on 2018. 10. 12..
//

#ifndef OPENGL_TESSELLATION_H
#define OPENGL_TESSELLATION_H

#include "Application.h"

class TessellationApp : public Application {
public:
    TessellationApp(
        std::string name,
        int width, int height)
        : Application(std::move(name), width, height) {}

    void onAcquireContext() override;
    void onReleaseContext() override;
    void onResize(int width, int height) override;
    bool onUpdate() override;
    void onDraw() override;

private:
    GLuint _program{0};
    GLuint _vao{0};
};


#endif //OPENGL_TESSELLATION_H
