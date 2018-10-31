//
// Created by blazeq on 28/10/2018.
//

#ifndef OPENGL_BLANKAPP_H
#define OPENGL_BLANKAPP_H


#include <Application.h>

class BlankApp : public Application {
public:
    BlankApp(
        std::string name,
        int width, int height)
        : Application(std::move(name), width, height) {}

    void onAcquireContext() override;
    void onResize(int width, int height) override;
    bool onUpdate() override;
    void onDraw() override;
};


#endif //OPENGL_BLANKAPP_H
