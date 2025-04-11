#pragma once
#include <test/3rdwrap/vulkan/vulkan_library.h>

#include <vulkan/vulkan_raii.hpp>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_VULKAN_BEGIN

struct Instance {
    ::vk::raii::Instance               instance  = nullptr;
    ::vk::raii::DebugUtilsMessengerEXT messenger = nullptr;
};

struct Device {
    ::vk::PhysicalDeviceGroupProperties physicalDeviceGroup;
    ::vk::raii::PhysicalDevice          physicalDevice     = nullptr;
    ::vk::raii::Device                  device             = nullptr;
    ::vk::raii::Queue                   graphicsQueue      = nullptr;
    uint64_t                            graphicsQueueIndex = 0;
    ::vk::raii::Queue                   transferQueue      = nullptr;
    uint64_t                            transferQueueIndex = 0;
    ::vk::raii::Queue                   computeQueue       = nullptr;
    uint64_t                            computeQueueIndex  = 0;
};

struct Swapchain {
    ::vk::raii::SwapchainKHR swapchain;
};

TEST_VULKAN_END
TEST_3RDWRAP_END
TEST_END