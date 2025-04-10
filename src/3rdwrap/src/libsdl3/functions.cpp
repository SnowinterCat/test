#include "test/3rdwrap/libsdl3/functions.hpp"

#include <SDL3/SDL_vulkan.h>

TEST_BEGIN
TEST_3RDWRAP_BEGIN
TEST_LIBSDL3_BEGIN

auto GetVkInstanceExtensions() -> std::span<const char *const>
{
    auto extenCnt   = uint32_t();
    auto extensions = SDL_Vulkan_GetInstanceExtensions(&extenCnt);
    return std::span(extensions, extenCnt);
}

auto TEST_3RDWRAP_API GetVkSurfaceOfWindow(SDL_Window *window, VkInstance instance,
                                           const VkAllocationCallbacks *allocator) -> VkSurfaceKHR
{
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(window, instance, allocator, &surface)) {
        return nullptr;
    }
    return surface;
}

TEST_LIBSDL3_END
TEST_3RDWRAP_END
TEST_END