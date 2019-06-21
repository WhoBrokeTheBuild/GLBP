#include <Program.hpp>
#include <Log.hpp>

#include <chrono>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

void Program::Run() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LogError("Failed to initialize SDL, %d", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

    sdl_window_ = SDL_CreateWindow("GLBP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!sdl_window_) {
        LogError("Failed to create SDL window, %s", SDL_GetError());
        return;
    }
    
    sdl_context_ = SDL_GL_CreateContext(sdl_window_);
    if (!sdl_context_) {
        LogError("Failed to create OpenGL context, %s", SDL_GetError());
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        LogError("Failed to initialize OpenGL context");
        return;
    }

    LogInfo("OpenGL Version %s", glGetString(GL_VERSION));
    LogInfo("GLSL Version %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    LogInfo("OpenGL Vendor %s", glGetString(GL_VENDOR));
    LogInfo("OpenGL Renderer %s", glGetString(GL_RENDERER));

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    using namespace std::chrono;
    typedef duration<double, std::milli> double_ms;

    unsigned long frames = 0;

    double_ms frameDelay = 1000ms / 60.0;
    double_ms fpsDelay = 250ms; // Update FPS 4 times per second

    double_ms frameElap = 0ms;
    double_ms fpsElap = 0ms;

    auto timeOffset = high_resolution_clock::now();

    SDL_Event evt;

    _running = true;
    while (_running) {
        auto elapsedTime = duration_cast<double_ms>(high_resolution_clock::now() - timeOffset);
        timeOffset = high_resolution_clock::now();

        while (SDL_PollEvent(&evt)) {
            switch (evt.type)
            {
            case SDL_QUIT:
                _running = false;
                break;
            case SDL_WINDOWEVENT:
                if (evt.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    glViewport(0, 0, evt.window.data1, evt.window.data2);
                }
                break;
            }
        }

        Update();

        frameElap += elapsedTime;
        if (frameDelay <= frameElap) {
            frameElap = 0ms;
            ++frames;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Render();

            SDL_GL_SwapWindow(sdl_window_);
        }
 
        fpsElap += elapsedTime;
        if (fpsDelay <= fpsElap)
        {
            float fps = (float)(frames / fpsElap.count()) * 1000.f;

            static char buffer[128];
            sprintf(buffer, "GLBP - %0.2f", fps);
            SDL_SetWindowTitle(sdl_window_, buffer);

            frames = 0;
            fpsElap = 0ms;
        }
    }
    
    SDL_GL_DeleteContext(sdl_context_);

    SDL_DestroyWindow(sdl_window_);
    sdl_window_ = nullptr;

    SDL_Quit();
}

void Program::Update() {

}

void Program::Render() {

}