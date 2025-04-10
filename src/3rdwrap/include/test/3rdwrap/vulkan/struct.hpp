#pragma once
#include <test/3rdwrap/vulkan/vulkan_library.h>

#include <vulkan/vulkan_raii.hpp>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_VULKAN_BEGIN

struct Instance {
    ::vk::raii::Instance               instance;
    ::vk::raii::DebugUtilsMessengerEXT messenger;
};

struct Device {
    ::vk::PhysicalDeviceGroupProperties physicalDeviceGroup;
    ::vk::raii::Device                  device;
    ::vk::raii::Queue                   graphicsQueue;
    uint64_t                            graphicsQueueIndex;
    ::vk::raii::Queue                   transferQueue;
    uint64_t                            transferQueueIndex;
    ::vk::raii::Queue                   computeQueue;
    uint64_t                            computeQueueIndex;
};

struct Swapchain {
    ::vk::raii::SwapchainKHR swapchain;
};

TEST_VULKAN_END
TEST_3RDWRAP_END
TEST_END