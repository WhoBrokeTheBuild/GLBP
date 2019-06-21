#pragma once

#include <depend/OpenGL.hpp>

class Program
{
public:

    static inline Program * Inst() {
        return inst_;
    };

    inline Program() {
        inst_ = this;
    }

    inline virtual ~Program() {
        inst_ = nullptr;
    }

    void Run();

    void Update();
    void Render();

private:

    inline static Program * inst_ = nullptr;

    inline static bool _running = false;

    inline static SDL_Window * sdl_window_ = nullptr;
    inline static SDL_GLContext sdl_context_;

};