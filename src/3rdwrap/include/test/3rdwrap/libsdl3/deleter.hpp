#pragma once
#include <test/3rdwrap/libsdl3/libsdl3_library.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_LIBSDL3_BEGIN

struct TEST_3RDWRAP_API Quiter {
    void operator()(void *p) { p ? SDL_Quit() : void(); }
};
struct TEST_3RDWRAP_API WindowDestroyer {
    void operator()(SDL_Window *p) { p ? SDL_DestroyWindow(p) : void(); }
};

void TEST_3RDWRAP_API unused();

TEST_LIBSDL3_END
TEST_3RDWRAP_END
TEST_END