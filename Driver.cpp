//
// Created by blazeq on 26/10/2018.
//

#include "Driver.h"
#include "Application.h"
#include <iostream>

using namespace std;

class DriverSdl : public Driver {
private:
    bool runApplication(Application& application) override {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cerr << "Failed to init SDL" << endl;
            return false;
        }

        SDL_Window* window = SDL_CreateWindow(
                application.name().c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                application.width(), application.height(),
                SDL_WINDOW_OPENGL);

        if (!window) {
            cerr << "Unable to create window" << endl;
            exitIfErrorOccurs(__LINE__);
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetSwapInterval(1);

        SDL_GLContext context = SDL_GL_CreateContext(window);

        const GLubyte* glVersion = glGetString(GL_VERSION);
        cout << "GL Version: " << glVersion  << endl;

        const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        cout << "GLSL Version: " << glslVersion  << endl;

        application.onCreate();
        application.onAcquireContext();
        application.onResize(application.width(), application.height());

        bool quit = false;
        while (!quit) {
            quit = application.onUpdate();
            application.onDraw();
            SDL_GL_SwapWindow(window);
        }

        application.onReleaseContext();
        application.onTerminate();

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return true;
    }

    void exitIfErrorOccurs(int line) {
        auto* error = SDL_GetError();
        if (error) {
            cerr << "Line: " << line << endl;
            cerr << error << endl;
            exit(1);
        }
    }
};

unique_ptr<Driver> Driver::create() {
    return unique_ptr<Driver>(new DriverSdl());
}