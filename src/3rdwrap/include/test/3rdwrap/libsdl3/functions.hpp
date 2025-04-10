#pragma once
#include <test/3rdwrap/libsdl3/libsdl3_library.h>

#include <span>

#include <SDL3/SDL_vulkan.h>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_LIBSDL3_BEGIN

auto TEST_3RDWRAP_API GetVkInstanceExtensions() -> std::span<const char *const>;

auto TEST_3RDWRAP_API GetVkSurfaceOfWindow(SDL_Window *window, VkInstance instance,
                                           const VkAllocationCallbacks *allocator = nullptr)
    -> VkSurfaceKHR;

TEST_LIBSDL3_END
TEST_3RDWRAP_END
TEST_END