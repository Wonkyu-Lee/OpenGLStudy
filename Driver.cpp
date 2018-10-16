//
// Created by blazeq on 2018. 10. 11..
//

#include <iostream>
#include "Application.h"

using namespace std;

class DriverSdl {
public:
    bool runApplication(Application& application) {
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

private:
    void exitIfErrorOccurs(int line) {
        auto* error = SDL_GetError();
        if (error) {
            cerr << "Line: " << line << endl;
            cerr << error << endl;
            exit(1);
        }
    }
};

int main(void) {
    auto application = unique_ptr<Application>(Application::create());
    auto driver = make_unique<DriverSdl>();
    return driver->runApplication(*application) ? 0 : 1;
}