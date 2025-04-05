#pragma once
#include <test/3rdwrap/3rdwrap_library.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

TEST_BEGIN
TEST_3RDWRAP_BEGIN

struct TEST_3RDWRAP_API Quiter {
    void operator()(void *p) { p ? SDL_Quit() : void(); }
};
struct TEST_3RDWRAP_API WindowDestroyer {
    void operator()(SDL_Window *p) { p ? SDL_DestroyWindow(p) : void(); }
};

void TEST_3RDWRAP_API unused();

TEST_3RDWRAP_END
TEST_END