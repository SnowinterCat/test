#include <test/config.h>
#include "luancher/u8main.hpp"

#include <span>

#include <spdlog/spdlog.h>

#include <test/vulkan.hpp>
#include <test/sdl3.hpp>

struct Context {
    int width  = 1280;
    int height = 720;

    vk::raii::Context                context;
    vk::raii::Instance               instance  = nullptr;
    vk::raii::DebugUtilsMessengerEXT messenger = nullptr;
    vk::raii::SurfaceKHR             surface   = nullptr;
} g_context;

//

void SetupVulkan(std::span<const char *const> extensions);

//

int u8main([[maybe_unused]] int argc, [[maybe_unused]] const char *const *argv)
{
    auto quiter = std::unique_ptr<void, test::sdl3::Quiter>(
        (void *)SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD));
    if (!quiter) {
        SPDLOG_ERROR("SDL_Init error, info: {}", SDL_GetError());
        return -1;
    }

    auto window = std::unique_ptr<SDL_Window, test::sdl3::WindowDestroyer>(
        SDL_CreateWindow("Test ImGui SDL3+Vulkan example", g_context.width, g_context.height,
                         SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY));
    if (!window) {
        SPDLOG_ERROR("SDL_CreateWindow error, info: {}", SDL_GetError());
        return -1;
    }

    SetupVulkan([]() -> auto {
        auto extenCnt   = uint32_t(0);
        auto extensions = SDL_Vulkan_GetInstanceExtensions(&extenCnt);
        return std::span(extensions, extenCnt);
    }());

    return 0;
}

void SetupVulkan(std::span<const char *const> extensions)
{
    SPDLOG_INFO("cnt: {}", extensions.size());
    for (uint32_t i = 0; i < extensions.size(); ++i) {
        SPDLOG_INFO("extension {}: {}", i, extensions[i]);
    }

    auto &context   = g_context.context;
    auto &instance  = g_context.instance;
    auto &messenger = g_context.messenger;

    auto version = context.enumerateInstanceVersion();
    SPDLOG_INFO("instance version: {}.{}.{}", vk::apiVersionMajor(version),
                vk::apiVersionMinor(version), vk::apiVersionPatch(version));

    auto layerProperties     = context.enumerateInstanceLayerProperties();
    auto extensionProperties = context.enumerateInstanceExtensionProperties();

    for (const auto &layerProperty : layerProperties) {
        auto specVersion = layerProperty.specVersion;
        SPDLOG_INFO("name: {}, des: {}, specVersion: {}.{}.{}", layerProperty.layerName.data(),
                    layerProperty.description.data(), vk::apiVersionMajor(specVersion),
                    vk::apiVersionMinor(specVersion), vk::apiVersionPatch(specVersion));
    }
    SPDLOG_INFO("");
    for (const auto &extensionProperty : extensionProperties) {
        auto specVersion = extensionProperty.specVersion;
        SPDLOG_INFO("name: {}, specVersion: {}.{}.{}", extensionProperty.extensionName.data(),
                    vk::apiVersionMajor(specVersion), vk::apiVersionMinor(specVersion),
                    vk::apiVersionPatch(specVersion));
    }

    auto appInfo           = vk::ApplicationInfo("",
                                                 vk::makeApiVersion(TEST_VERSION_MAJOR, TEST_VERSION_MINOR,
                                                                    TEST_VERSION_ALTER, TEST_VERSION_BUILD),
                                                 "No Engine",
                                                 vk::makeApiVersion(TEST_VERSION_MAJOR, TEST_VERSION_MINOR,
                                                                    TEST_VERSION_ALTER, TEST_VERSION_BUILD),
                                                 vk::ApiVersion12);
    auto enabledLayers     = std::vector<const char *>();
    auto enabledExtensions = std::vector<const char *>();

    auto info = vk::InstanceCreateInfo({}, &appInfo, enabledLayers, enabledExtensions);
    instance  = context.createInstance(info).value();
}