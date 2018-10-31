//
// Created by blazeq on 28/10/2018.
//

#include "BlankApp.h"
#include <catch2/catch.hpp>
#include "Driver.h"

void BlankApp::onAcquireContext() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void BlankApp::onResize(int width, int height) {
    glViewport(0, 0, width, height);
}

bool BlankApp::onUpdate() {
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

void BlankApp::onDraw() {
    glClear(GL_COLOR_BUFFER_BIT);
}

TEST_CASE("Blank", "[blank]") {
    Driver::run<BlankApp>("blank", 512, 512);
}