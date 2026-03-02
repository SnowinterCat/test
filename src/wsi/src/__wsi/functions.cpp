#include <test/__wsi/functions.hpp>
// Standard Library
// System Library
// Third-Party Library
// Local Library

TEST_BEGIN
TEST_WSI_BEGIN

auto TEST_WSI_API getVkInstanceExtensions() -> std::span<const char *const>
{
    auto        extenCnt   = uint32_t();
    const auto *extensions = SDL_Vulkan_GetInstanceExtensions(&extenCnt);
    return std::span(extensions, extenCnt);
}

auto TEST_WSI_API getVkSurfaceOfWindow(SDL_Window *window, VkInstance instance,
                                       const VkAllocationCallbacks *allocator) -> VkSurfaceKHR
{
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(window, instance, allocator, &surface)) {
        return nullptr;
    }
    return surface;
}

TEST_WSI_END
TEST_END