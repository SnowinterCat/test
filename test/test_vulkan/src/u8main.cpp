#include <test/config.hpp>
#include "luancher/u8main.hpp"
// Standard Library
#include <span>
// System Library
// Third-Party Library
#include <spdlog/spdlog.h>
// Local Library
#include <test/rhi.hpp>
#include <test/wsi.hpp>

struct Context {
    int width  = 1280;
    int height = 720;

    vk::raii::Context                context;
    vk::raii::Instance               instance  = nullptr;
    vk::raii::DebugUtilsMessengerEXT messenger = nullptr;
    vk::raii::SurfaceKHR             surface   = nullptr;
} g_context;

//

auto setupVulkan(std::span<const char *const> extensions) -> vk::Result;

//

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    using namespace test;

    auto sdlContext = wsi::raii::Context((void *)SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD));
    if (!sdlContext) {
        SPDLOG_ERROR("SDL_Init error, info: {}", SDL_GetError());
        return -1;
    }
    auto sdlWindow = wsi::raii::Window(
        SDL_CreateWindow("Test ImGui SDL3+Vulkan example", g_context.width, g_context.height,
                         SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY));
    if (!sdlWindow) {
        SPDLOG_ERROR("SDL_CreateWindow error, info: {}", SDL_GetError());
        return -1;
    }

    // create instance, device, descriptor pool
    auto vkErrc = vk::Result();
    if (vkErrc = setupVulkan(wsi::getVkInstanceExtensions()); vkErrc != vk::Result::eSuccess) {
        SPDLOG_ERROR("setupVulkan error, code: {}, info: {}", static_cast<int>(vkErrc),
                     vk::to_string(vkErrc));
        return static_cast<int>(vkErrc);
    }

    // create surface
    g_context.surface = vk::raii::SurfaceKHR(
        g_context.instance, wsi::getVkSurfaceOfWindow(sdlWindow.get(), *g_context.instance));
    if (*g_context.surface == nullptr) {
        SPDLOG_ERROR("getVkSurfaceOfWindow error, info: {}", SDL_GetError());
        return static_cast<int>(vk::Result::eErrorSurfaceLostKHR);
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
                if (event.window.windowID == SDL_GetWindowID(sdlWindow.get())) {
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

auto setupVulkan(std::span<const char *const> extensions) -> vk::Result
{
    using namespace test;

    auto  vkErrc    = vk::Result();
    auto &context   = g_context.context;
    auto &instance  = g_context.instance;
    auto &messenger = g_context.messenger;
    // auto &device   = g_context.device;

    // Create Vulkan Instance and Debug Messenger
    std::tie(vkErrc, instance, messenger) =
        rhi::createVkRaiiInstanceAndMessengerDefault(context, extensions, true);
    if (vkErrc != vk::Result::eSuccess) {
        return vkErrc;
    }

    // Vulkan Physical Device and Logical Device
    // std::tie(vkErrc, device) = = rhi::CreateDefaultDevice(instance, {});
    // if (vkErrc != vk::Result::eSuccess) {
    //     return vkErrc;
    // }

    // Vulkan Descriptor Pool
    return {};
}
