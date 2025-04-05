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

#define VULKAN_VALIDATION_ENABLE true

void SetupVulkan(std::span<const char *const> extensions);

static bool IsLayerAvailable(const std::vector<vk::LayerProperties> &properties, const char *layer);
static bool IsExtensionAvailable(const std::vector<vk::ExtensionProperties> &properties,
                                 const char                                 *extension);

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/);

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

void SetupVulkan(std::span<const char *const> extensions)
{
    auto &context   = g_context.context;
    auto &instance  = g_context.instance;
    auto &messenger = g_context.messenger;

    auto version             = context.enumerateInstanceVersion();
    auto layerProperties     = context.enumerateInstanceLayerProperties();
    auto extensionProperties = context.enumerateInstanceExtensionProperties();

    SPDLOG_INFO("instance version: {}.{}.{}", vk::apiVersionMajor(version),
                vk::apiVersionMinor(version), vk::apiVersionPatch(version));
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
    auto enabledExtensions = std::vector<const char *>(extensions.begin(), extensions.end());

    auto vdEnabled = false;
#if defined(VULKAN_VALIDATION_ENABLE) && VULKAN_VALIDATION_ENABLE
    vdEnabled = true;
    if (IsLayerAvailable(layerProperties, "VK_LAYER_KHRONOS_validation")) {
        enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
    }
    if (IsLayerAvailable(layerProperties, "VK_LAYER_LUNARG_monitor")) {
        enabledLayers.push_back("VK_LAYER_LUNARG_monitor");
    }
    if (IsExtensionAvailable(extensionProperties, VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
        enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    auto messengerInfo = vk::DebugUtilsMessengerCreateInfoEXT(
        {},
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
        &DebugMessengerCallback, nullptr);
    auto vdFeatureEnable = {vk::ValidationFeatureEnableEXT::eDebugPrintf};
    auto vdFeatures      = vk::ValidationFeaturesEXT(vdFeatureEnable, {}, &messengerInfo);
#endif

    auto instanceInfo = vk::InstanceCreateInfo({}, &appInfo, enabledLayers, enabledExtensions,
                                               vdEnabled ? &vdFeatures : nullptr);

    instance  = context.createInstance(instanceInfo).value();
    messenger = instance.createDebugUtilsMessengerEXT(messengerInfo).value();
}

bool IsLayerAvailable(const std::vector<vk::LayerProperties> &properties, const char *layer)
{
    for (const auto &p : properties)
        if (strcmp(p.layerName, layer) == 0)
            return true;
    return false;
}

bool IsExtensionAvailable(const std::vector<vk::ExtensionProperties> &properties,
                          const char                                 *extension)
{
    for (const auto &p : properties)
        if (strcmp(p.extensionName, extension) == 0)
            return true;
    return false;
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/)
{
    if (static_cast<uint32_t>(pCallbackData->messageIdNumber) == 0x822806fa) {
        // Validation Warning: vkCreateInstance(): to enable extension VK_EXT_debug_utils,
        // but this extension is intended to support use by applications when
        // debugging and it is strongly recommended that it be otherwise avoided.
        return vk::False;
    }
    else if (static_cast<uint32_t>(pCallbackData->messageIdNumber) == 0xe8d1a9fe) {
        // Validation Performance Warning: Using debug builds of the validation layers *will*
        // adversely affect performance.
        return vk::False;
    }
    if (static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity) ==
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo &&
        strcmp(pCallbackData->pMessageIdName, "WARNING-DEBUG-PRINTF") != 0) {
        // 忽略 info 等级中，除了 vk::ValidationFeatureEnableEXT::eDebugPrintf 的所有消息
        return vk::False;
    }

    auto tmp = fmt::format(
        "\n{}: ( {} ):\n\tmessageIDName   = <{}>\n\tmessageIdNumber = {}\n\tmessage         = <{}>",
        vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)),
        vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)),
        pCallbackData->pMessageIdName, pCallbackData->messageIdNumber, pCallbackData->pMessage);
    if (0 < pCallbackData->queueLabelCount) {
        tmp = fmt::format("{}\n\tQueue Labels:", tmp);
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; ++i) {
            tmp = fmt::format("{}\n\t\tlabelName = <{}>", tmp,
                              pCallbackData->pQueueLabels[i].pLabelName);
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount) {
        tmp = fmt::format("{}\n\tCommandBuffer Labels:", tmp);
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            tmp = fmt::format("{}\n\t\tlabelName = <{}>", tmp,
                              pCallbackData->pCmdBufLabels[i].pLabelName);
        }
    }
    if (0 < pCallbackData->objectCount) {
        tmp = fmt::format("{}\n\tObjects:", tmp);
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
            tmp = fmt::format(
                "{}\n\t\tObject {}\n\t\t\tobjectType   = {}\n\t\t\tobjectHandle = {}", tmp, i,
                vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)),
                pCallbackData->pObjects[i].objectHandle);
            if (pCallbackData->pObjects[i].pObjectName) {
                tmp = fmt::format("{}\n\t\t\tobjectName   = <{}>", tmp,
                                  pCallbackData->pObjects[i].pObjectName);
            }
        }
    }

    auto logLevel = spdlog::level::level_enum();
    switch (static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) {
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
        logLevel = spdlog::level::debug;
        break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
        logLevel = spdlog::level::info;
        break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
        logLevel = spdlog::level::warn;
        break;
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
        logLevel = spdlog::level::err;
        break;
    }
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), logLevel, "{}", tmp);
    return vk::False;
}
