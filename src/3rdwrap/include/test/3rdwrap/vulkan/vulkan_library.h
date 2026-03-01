#ifndef _TEST_3RDWRAP_VULKAN_LIBRARY_H_
#define _TEST_3RDWRAP_VULKAN_LIBRARY_H_
#include <test/__3rdwrap/__config.hpp>

#define TEST_VULKAN_BEGIN                                                                          \
    namespace vk                                                                                   \
    {
#define TEST_VULKAN_END }

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC true

#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_RAII_NO_EXCEPTIONS

#endif