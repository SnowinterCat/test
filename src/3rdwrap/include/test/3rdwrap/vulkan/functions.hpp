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

auto TEST_3RDWRAP_API CreateDefaultDevice(const Instance &, const ::vk::PhysicalDeviceFeatures &)
    -> std::expected<Device, ::vk::Result>;

auto TEST_3RDWRAP_API CreateDefaultSwapchain(const Device &)
    -> std::expected<Swapchain, ::vk::Result>;

//

bool TEST_3RDWRAP_API IsLayerAvailable(const std::vector<::vk::LayerProperties> &properties,
                                       const char                               *layer);
bool TEST_3RDWRAP_API IsExtensionAvailable(const std::vector<::vk::ExtensionProperties> &properties,
                                           const char                                   *extension);

//

VKAPI_ATTR VkBool32 VKAPI_CALL DefaultDebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/);

TEST_VULKAN_END
TEST_3RDWRAP_END
TEST_END