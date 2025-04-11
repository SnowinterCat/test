#pragma once
#include <test/3rdwrap/vulkan/vulkan_library.h>

#include <expected>
#include <span>

#include <vulkan/vulkan_raii.hpp>

#include "test/3rdwrap/vulkan/struct.hpp"

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_VULKAN_BEGIN

/**
 * @brief
 *
 * @param context
 * @param extensions
 * @param isDebug
 * @return std::expected<Instance, ::vk::Result>
 */
auto TEST_3RDWRAP_API CreateDefaultVkInstance(const ::vk::raii::Context   &context,
                                              std::span<const char *const> extensions, bool isDebug)
    -> std::expected<Instance, ::vk::Result>;

auto TEST_3RDWRAP_API CreateDefaultDevice(const Instance                     &instance,
                                          const ::vk::PhysicalDeviceFeatures &features)
    -> std::expected<Device, ::vk::Result>;

auto TEST_3RDWRAP_API CreateDefaultSwapchain(const Device &)
    -> std::expected<Swapchain, ::vk::Result>;

//

bool TEST_3RDWRAP_API IsLayerAvailable(const std::vector<::vk::LayerProperties> &properties,
                                       const char                               *layer);
bool TEST_3RDWRAP_API IsExtensionAvailable(const std::vector<::vk::ExtensionProperties> &properties,
                                           const char                                   *extension);

auto TEST_3RDWRAP_API SelectPhysicalDeviceGroup(
    const std::vector<::vk::PhysicalDeviceGroupProperties> &properties,
    const ::vk::raii::Instance &instance) -> ::vk::PhysicalDeviceGroupProperties;
auto TEST_3RDWRAP_API
SelectDeviceQueueFamilyIndex(const std::vector<::vk::QueueFamilyProperties> &properties,
                             ::vk::QueueFlags required, ::vk::QueueFlags refused) -> size_t;
void TEST_3RDWRAP_API
SelectDeviceQueueIndex(const std::vector<::vk::QueueFamilyProperties> &properties,
                       std::span<uint64_t *const>                      queueIndices);

auto TEST_3RDWRAP_API BuildDeviceQueueCreateInfo(::vk::DeviceQueueCreateFlags deviceQueueCreateFlag,
                                                 std::span<uint64_t>          queueIndices,
                                                 std::span<float>             queuePriorities)
    -> std::vector<::vk::DeviceQueueCreateInfo>;

//

VKAPI_ATTR VkBool32 VKAPI_CALL DefaultDebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/);

TEST_VULKAN_END
TEST_3RDWRAP_END
TEST_END