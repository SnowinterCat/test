#pragma once
#include <test/__wsi/__config.hpp>
// Standard Library
#include <span>
// System Library
// Third-Party Library
#include <SDL3/SDL_vulkan.h>
// Local Library

TEST_BEGIN
TEST_WSI_BEGIN

auto TEST_WSI_API getVkInstanceExtensions() -> std::span<const char *const>;

auto TEST_WSI_API getVkSurfaceOfWindow(SDL_Window *window, VkInstance instance,
                                       const VkAllocationCallbacks *allocator = nullptr)
    -> VkSurfaceKHR;

TEST_WSI_END
TEST_END