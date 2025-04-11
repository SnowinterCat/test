#include <test/config.h>
#include "luancher/u8main.hpp"

#include <span>

#include <spdlog/spdlog.h>

#include <test/3rdwrap.hpp>

struct Context {
    int width  = 1280;
    int height = 720;

    vk::raii::Context        context;
    test::wrap::vk::Instance instance;
    test::wrap::vk::Device   device;
} g_context;

//

auto SetupVulkan(std::span<const char *const> extensions) -> vk::Result;

//

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    auto quiter = std::unique_ptr<void, test::wrap::sdl3::Quiter>(
        (void *)SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD));
    if (!quiter) {
        SPDLOG_ERROR("SDL_Init error, info: {}", SDL_GetError());
        return -1;
    }

    auto window = std::unique_ptr<SDL_Window, test::wrap::sdl3::WindowDestroyer>(
        SDL_CreateWindow("Test ImGui SDL3+Vulkan example", g_context.width, g_context.height,
                         SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY));
    if (!window) {
        SPDLOG_ERROR("SDL_CreateWindow error, info: {}", SDL_GetError());
        return -1;
    }

    auto vkRes = vk::Result();
    if (vkRes = SetupVulkan(test::wrap::sdl3::GetVkInstanceExtensions());
        vkRes != vk::Result::eSuccess) {
        SPDLOG_ERROR("SetupVulkan error, code: {}, info: {}", static_cast<int>(vkRes),
                     vk::to_string(vkRes));
        return static_cast<int>(vkRes);
    }

    auto event = SDL_Event();
    auto run   = true;
    while (run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                run = false;
                SPDLOG_INFO("Get Message: SDL_EVENT_QUIT");
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(window.get())) {
                    run = false;
                }
                SPDLOG_INFO("Get Message: SDL_EVENT_WINDOW_CLOSE_REQUESTED");
                break;
            default:
                SPDLOG_INFO("Get Message: {}", event.type);
                break;
            }
        }
        // SPDLOG_INFO("Render");
    }
    return 0;
}

auto SetupVulkan(std::span<const char *const> extensions) -> vk::Result
{
    auto &context  = g_context.context;
    auto &instance = g_context.instance;
    auto &device   = g_context.device;

    // Create Vulkan Instance and Debug Messenger
    if (auto res = test::wrap::vk::CreateDefaultVkInstance(context, extensions, true); !res)
        return res.error();
    else {
        instance = std::move(res.value());
    }
    if (auto res = test::wrap::vk::CreateDefaultDevice(instance, {}); !res)
        return res.error();
    else {
        device = std::move(res.value());
    }

    // Physical Device and Logical Device
    return vk::Result();
}
