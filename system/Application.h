//
// Created by blazeq on 2018. 10. 11..
//

#ifndef OPENGL_APPLICATION_H
#define OPENGL_APPLICATION_H

#include "OpenGl.h"
#include <memory>

class Application {
public:
    const std::string& name() const { return _name; }
    int width() const { return _width; }
    int height() const { return _height; }

    Application(
            std::string name,
            int width, int height):
            _name(std::move(name)),
            _width(width),
            _height(height) {}

    virtual ~Application() = default;
    virtual void onCreate() {}
    virtual void onAcquireContext() {}
    virtual void onResize(int width, int height) {}
    virtual bool onUpdate() { return false; }
    virtual void onDraw() {}
    virtual void onReleaseContext() {}
    virtual void onTerminate() {}

private:
    std::string _name;
    int _width;
    int _height;
};

#endif //OPENGL_APPLICATION_H
