#pragma once
#include <test/__rhi/__config.hpp>
// Standard Library
// System Library
// Third-Party Library
#include <vulkan/vulkan_raii.hpp>
// Local Library

TEST_BEGIN
TEST_RHI_BEGIN

auto TEST_RHI_API createVkRaiiInstanceAndMessengerDefault(const ::vk::raii::Context   &context,
                                                          std::span<const char *const> extensions,
                                                          bool debug = false)
    -> std::tuple<::vk::Result, ::vk::raii::Instance, ::vk::raii::DebugUtilsMessengerEXT>;

TEST_RHI_END
TEST_END