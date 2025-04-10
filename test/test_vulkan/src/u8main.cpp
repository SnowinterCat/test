#include <test/config.h>
#include "luancher/u8main.hpp"

#include <span>

#include <spdlog/spdlog.h>

#include <test/3rdwrap.hpp>

struct Context {
    int width  = 1280;
    int height = 720;

    vk::PhysicalDeviceGroupProperties physicalDeviceGroup;

    vk::raii::Context                context;
    vk::raii::Instance               instance  = nullptr;
    vk::raii::DebugUtilsMessengerEXT messenger = nullptr;
    vk::raii::Device                 device    = nullptr;
    vk::raii::SurfaceKHR             surface   = nullptr;

    vk::raii::Queue graphicsQueue = nullptr;
    vk::raii::Queue transferQueue = nullptr;
    vk::raii::Queue computeQueue  = nullptr;
} g_context;

//

auto SetupVulkan(std::span<const char *const> extensions) -> vk::Result;

static size_t SelectQueueFamilyIndex(const std::vector<vk::QueueFamilyProperties> &properties,
                                     vk::QueueFlags requiredFlags, vk::QueueFlags refused);

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
    auto &physicalDeviceGroup = g_context.physicalDeviceGroup;

    auto &context   = g_context.context;
    auto &instance  = g_context.instance;
    auto &messenger = g_context.messenger;
    auto &device    = g_context.device;
    (void)messenger;

    // Create Vulkan Instance and Debug Messenger
    if (auto res = test::wrap::vk::CreateDefaultVkInstance(context, extensions, true); !res)
        return res.error();
    else {
        instance  = std::move(res->instance);
        messenger = std::move(res->messenger);
    }

    // Physical Device and Logical Device
    {
        auto deviceGroups = instance.enumeratePhysicalDeviceGroups();
        for (const auto &deviceGroup : deviceGroups) {
            auto property = deviceGroup.physicalDevices[0].getProperties(*instance.getDispatcher());
            if (property.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                physicalDeviceGroup = deviceGroup;
            }
        }
        auto physicalDevice =
            vk::raii::PhysicalDevice(instance, physicalDeviceGroup.physicalDevices[0]);

        //
        auto     queueProperties = physicalDevice.getQueueFamilyProperties();
        uint32_t graphicsQueueFamily =
            SelectQueueFamilyIndex(queueProperties, vk::QueueFlagBits::eGraphics, vk::QueueFlags());
        uint32_t transferQueueFamily = SelectQueueFamilyIndex(
            queueProperties, vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics);
        uint32_t computeQueueFamily =
            SelectQueueFamilyIndex(queueProperties, vk::QueueFlagBits::eCompute, vk::QueueFlags());

        for (const auto &queueProperty : queueProperties) {
            SPDLOG_INFO(
                "queue count: {}, flags: {}, minImageTransferGranularity: {}*{}*{}, timestamp: {}",
                queueProperty.queueCount, vk::to_string(queueProperty.queueFlags),
                queueProperty.minImageTransferGranularity.width,
                queueProperty.minImageTransferGranularity.height,
                queueProperty.minImageTransferGranularity.depth, queueProperty.timestampValidBits);
        }
        SPDLOG_INFO("graphics: {}, transfer: {}, compute: {}", graphicsQueueFamily,
                    transferQueueFamily, computeQueueFamily);

        auto deviceGroupInfo = vk::DeviceGroupDeviceCreateInfo(physicalDeviceGroup.physicalDevices);
        auto deviceInfo      = vk::DeviceCreateInfo();

        if (auto res = physicalDevice.createDevice(deviceInfo); res)
            device = std::move(res.value());
        else {
            return res.error();
        }

        // device.getQueue(, );
    }
    return vk::Result();
}

size_t SelectQueueFamilyIndex(const std::vector<vk::QueueFamilyProperties> &properties,
                              vk::QueueFlags requiredFlags, vk::QueueFlags refused)
{
    for (size_t i = 0; i < properties.size(); ++i) {
        if (properties[i].queueFlags & requiredFlags && !(properties[i].queueFlags & refused))
            return i;
    }
    return -1;
}