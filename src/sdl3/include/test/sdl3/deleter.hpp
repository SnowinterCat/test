#pragma once
#include <test/sdl3/sdl3_library.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

TEST_BEGIN
TEST_SDL3_BEGIN

struct Quiter {
    void operator()(void *p) { p ? SDL_Quit() : void(); }
};
struct WindowDestroyer {
    void operator()(SDL_Window *p) { p ? SDL_DestroyWindow(p) : void(); }
};

void TEST_SDL3_API unused();

TEST_SDL3_END
TEST_END