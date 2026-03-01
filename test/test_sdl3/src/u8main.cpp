#include <test/config.hpp>
#include "luancher/u8main.hpp"
// Standard Library
#include <memory>
// System Library
// Third-Party Library
#include <spdlog/spdlog.h>
// Local Library
#include <test/wsi.hpp>

using namespace test;

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    auto sdlContext = wsi::raii::Context((void *)SDL_Init(SDL_INIT_VIDEO));
    if (!sdlContext) {
        SPDLOG_INFO("SDL_Init error, info: {}", SDL_GetError());
        return 0;
    }

    auto sdlWindow =
        wsi::raii::Window(SDL_CreateWindow("test_sdl3", 800, 600,
                                           SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                                               SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN));
    if (!sdlWindow) {
        SPDLOG_INFO("SDL_CreateWindow error, info: {}", SDL_GetError());
        return 0;
    }
    SDL_SetWindowPosition(sdlWindow.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(sdlWindow.get());

    {
        int w, h;
        SDL_GetWindowSize(sdlWindow.get(), &w, &h);
        SPDLOG_INFO("Window: {}*{}", w, h);
    }

    auto done = false;
    while (!done) {
        auto event = SDL_Event();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                done = true;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(sdlWindow.get()))
                    done = true;
                break;
            default:
                break;
            }
            SPDLOG_INFO("Get Message: {}", event.type);
        }
        // SPDLOG_INFO("Render");
    }
    return 0;
}
