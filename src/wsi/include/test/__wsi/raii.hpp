#pragma once
#include <test/__wsi/__config.hpp>
// Standard Library
#include <memory>
// System Library
// Third-Party Library
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
// Local Library

TEST_BEGIN
TEST_WSI_BEGIN

struct TEST_WSI_API ContextDeleter {
    void operator()(void *p);
};
struct TEST_WSI_API WindowDeleter {
    void operator()(SDL_Window *p);
};

namespace raii
{
    using Context = std::unique_ptr<void, ContextDeleter>;
    using Window  = std::unique_ptr<SDL_Window, WindowDeleter>;
} // namespace raii

TEST_WSI_END
TEST_END