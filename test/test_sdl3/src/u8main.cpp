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
} // namespace sdl3

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    auto sdlIniter =
        std::unique_ptr<void, sdl3::Quiter>((void *)SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    SPDLOG_INFO("{}", sdlIniter.get());

    return 0;
}
