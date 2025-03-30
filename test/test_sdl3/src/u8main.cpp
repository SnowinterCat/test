#include <test/config.h>
#include "luancher/u8main.hpp"

#include <memory>

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

namespace sdl3
{
    struct Quiter {
        void operator()([[maybe_unused]] void *p) { SDL_Quit(); }
    };

    struct WindowDeleter {
        void operator()(SDL_Window *window)
        {
            if (window) {
                SDL_DestroyWindow(window);
            }
        }
    };
} // namespace sdl3

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    auto sdlIniter =
        std::unique_ptr<void, sdl3::Quiter>((void *)SDL_Init(SDL_INIT_VIDEO));
    if (!sdlIniter) {
        SPDLOG_INFO("SDL_Init error, info: {}", SDL_GetError());
        return 0;
    }

    auto sdlWindow = std::unique_ptr<SDL_Window, sdl3::WindowDeleter>(
        SDL_CreateWindow("title", 800, 600, SDL_WINDOW_VULKAN));
    if (!sdlWindow) {
        SPDLOG_INFO("SDL_CreateWindow error, info: {}", SDL_GetError());
        return 0;
    }

    auto event = SDL_Event();
    auto run   = true;
    while (run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                run = false;
                break;
            default:
                SPDLOG_INFO("Get Message: {}", event.type);
                break;
            }
        }
        SPDLOG_INFO("Render");
    }
    return 0;
}
